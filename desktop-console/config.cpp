#include "config.h"

#include "../rtl/UILib.h"
#include "../rtl/FilterLib.h"
#include "../lang/dstrings.h"

#include "../rtl/FilesystemLib.h"

#include <vector>
#include <list>
#include <fstream>

CConfig Configuration;

void CConfig::Resolve_And_Load_Config_File() {
	mFile_Path = Get_Application_Dir();
	mFile_Name = rsConfig_File_Name;

	Path_Append(mFile_Path, mFile_Name.c_str());

	std::vector<char> buf;
	std::ifstream configfile;

	try {
		configfile.open(mFile_Path);

		if (configfile.is_open()) {
			buf.assign(std::istreambuf_iterator<char>(configfile), std::istreambuf_iterator<char>());
			mIni.LoadData(buf.data(), buf.size());
		}

		configfile.close();
	}
	catch (...) {
	}
}

const wchar_t* CConfig::Get_Config_File_Name() const
{
	return mFile_Name.c_str();
}

void CConfig::Load(CFilter_Chain &filter_chain) {
	std::list<CSimpleIniW::Entry> section_names;
	mIni.GetAllSections(section_names);

	// sort by section names - the name would contain zero-padded number, so it is possible to sort it as strings
	section_names.sort([](auto& a, auto& b) {
		return std::wstring(a.pItem).compare(b.pItem) < 0;
	});

	CFilter_Chain new_chain;
	for (auto &section_name : section_names) {
		std::wstring name_str{ section_name.pItem };
		const std::wstring prefix{ rsFilter_Section_Prefix };
		auto res = std::mismatch(prefix.begin(), prefix.end(), name_str.begin());
		if (res.first == prefix.end()) {

			auto uspos = name_str.find(rsFilter_Section_Separator, prefix.size() + 1);
			if (uspos == std::wstring::npos)
				uspos = prefix.size();

			//OK, this is filter section - extract the guid
			const GUID id = WString_To_GUID(std::wstring{ name_str.begin() + uspos + 1, name_str.end() });
			//and get the filter descriptor to load the parameters
			
			glucose::TFilter_Descriptor desc{ 0 };
			CFilter_Configuration filter_config;

			if (glucose::get_filter_descriptors_by_id(id, desc)) {
				//so.. now, try to load the filter parameters - aka filter_config

				for (size_t i = 0; i < desc.parameters_count; i++) {
					glucose::TFilter_Parameter filter_parameter;
					filter_parameter.type = desc.parameter_type[i];
					filter_parameter.config_name = refcnt::WString_To_WChar_Container(desc.config_parameter_name[i]);

					//does the value exists?
					const wchar_t* str_value = mIni.GetValue(section_name.pItem, desc.config_parameter_name[i]);
					if (str_value) {

						bool valid = true;

						//yes, there is somethign stored under this key
						switch (filter_parameter.type)
						{
							case glucose::NParameter_Type::ptWChar_Container:
								filter_parameter.wstr = refcnt::WString_To_WChar_Container(str_value);
								break;

							case glucose::NParameter_Type::ptSelect_Time_Segment_ID:
								filter_parameter.select_time_segment_id = WString_To_Select_Time_Segments_Id(str_value);
								break;

							case glucose::NParameter_Type::ptDouble:
								filter_parameter.dbl = mIni.GetDoubleValue(section_name.pItem, desc.config_parameter_name[i]);
								break;

							case glucose::NParameter_Type::ptInt64:
								filter_parameter.int64 = mIni.GetLongValue(section_name.pItem, desc.config_parameter_name[i]);
								break;

							case glucose::NParameter_Type::ptBool:
								filter_parameter.boolean = mIni.GetBoolValue(section_name.pItem, desc.config_parameter_name[i]);
								break;

							case glucose::NParameter_Type::ptModel_Id:
							case glucose::NParameter_Type::ptMetric_Id:
							case glucose::NParameter_Type::ptModel_Signal_Id:
							case glucose::NParameter_Type::ptSignal_Id:
							case glucose::NParameter_Type::ptSolver_Id:
								filter_parameter.guid = WString_To_GUID(str_value);
								break;

							case glucose::NParameter_Type::ptModel_Bounds:
								filter_parameter.parameters = WString_To_Model_Parameters(str_value);
								break;

							default:
								valid = false;
						}

						if (valid)
							filter_config.push_back(filter_parameter);
					}
				}

				//and finally, add the new link into the filter chain
				new_chain.push_back({ desc, filter_config });
			}
		}
	}

	//finally, replace the chain with new one
	filter_chain = std::move(new_chain);
}

void CConfig::Save(const CFilter_Chain &filter_chain) {
	
	uint32_t i = 1;

	// for now, reset the ini file contents; this is to avoid duplicating filter records on delete - i.e. when deleting one filter and the rest just "moves" up by one position,
	// technically the rest would have different identifiers, so the code would leave them here and potentially duplicate them; TODO: rework to selective deletion
	mIni.Reset();

	for (auto &link : filter_chain) {
		const std::wstring id_str = std::wstring(rsFilter_Section_Prefix) + rsFilter_Section_Separator + Get_Padded_Number(i++, 3) + rsFilter_Section_Separator + GUID_To_WString(link.descriptor.id);
		auto section = mIni.GetSection(id_str.c_str());
		if (!section) 
			//if the section does not exist yet, create it by writing a comment there - the filter description
			mIni.SetValue(id_str.c_str(), nullptr, nullptr, std::wstring{ rsIni_Comment_Prefix}.append(link.descriptor.description).c_str());

		//and store the parameters
		for (const auto &param : link.configuration) {
			switch (param.type) {
				case glucose::NParameter_Type::ptWChar_Container: 
					mIni.SetValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), WChar_Container_To_WString(param.wstr).c_str());
					break;

				case glucose::NParameter_Type::ptSelect_Time_Segment_ID:
					mIni.SetValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), Select_Time_Segments_Id_To_WString(param.select_time_segment_id).c_str());
					break;

				case glucose::NParameter_Type::ptDouble:
					mIni.SetDoubleValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), param.dbl);
					break;

				case glucose::NParameter_Type::ptInt64:
					mIni.SetLongValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), static_cast<long>(param.int64));
					break;

				case glucose::NParameter_Type::ptBool:
					mIni.SetBoolValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), param.boolean);
					break;

				case glucose::NParameter_Type::ptModel_Id:
				case glucose::NParameter_Type::ptMetric_Id:
				case glucose::NParameter_Type::ptModel_Signal_Id:
				case glucose::NParameter_Type::ptSignal_Id:
				case glucose::NParameter_Type::ptSolver_Id:
					mIni.SetValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), GUID_To_WString(param.guid).c_str());
					break;

				case glucose::NParameter_Type::ptModel_Bounds:
					mIni.SetValue(id_str.c_str(), WChar_Container_To_WString(param.config_name).c_str(), Model_Parameters_To_WString(param.parameters).c_str());
					break;
			}
		}
			
		
	}

	std::string content;
	mIni.Save(content);
	std::ofstream config_file(mFile_Path);
	if (config_file.is_open()) {
		config_file << content;
		config_file.close();
	}
}
