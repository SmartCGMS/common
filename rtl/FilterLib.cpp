/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8
 * 301 00, Pilsen
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "FilterLib.h"

#include "FactoryLib.h"

#include <sstream>
#include "manufactory.h"

namespace glucose {


	namespace imported {
		glucose::TGet_Filter_Descriptors get_filter_descriptors = factory::resolve_symbol<glucose::TGet_Filter_Descriptors>("get_filter_descriptors");
		glucose::TAdd_Filters add_filters = factory::resolve_symbol<glucose::TAdd_Filters>("add_filters");
		glucose::TCreate_Persistent_Filter_Chain_Configuration create_persistent_filter_chain_configuration = factory::resolve_symbol<glucose::TCreate_Persistent_Filter_Chain_Configuration>("create_persistent_filter_chain_configuration");		
		glucose::TExecute_Filter_Configuration execute_filter_configuration = factory::resolve_symbol<glucose::TExecute_Filter_Configuration>("execute_filter_configuration");
	}


	std::wstring SFilter_Parameter::as_string(HRESULT &rc) {
		std::wstring result = nullptr;

		refcnt::wstr_container* wstr;
		rc = get()->Get_WChar_Container(&wstr);
		if (rc == S_OK) {
			result = refcnt::WChar_Container_To_WString(wstr);
			wstr->Release();
		}

		return result;
	}

	void SFilter_Parameter::set_string(const wchar_t *str, HRESULT &rc) {
		std::shared_ptr<refcnt::wstr_container> wstr = refcnt::WString_To_WChar_Container_shared(str);
		rc = get()->Set_WChar_Container(wstr.get());
	}

	int64_t SFilter_Parameter::as_int(HRESULT &rc) {
		int64_t result = 0;
		rc = get()->Get_Int64(&result);	//if fails, it returns 0
		return result;
	}

	void SFilter_Parameter::set_int(const int64_t value, HRESULT &rc) {
		rc = get()->Set_Int64(value);
	}


	double SFilter_Parameter::as_double(HRESULT &rc) {
		double result = std::numeric_limits<double>::quiet_NaN();
		rc = get()->Get_Double(&result);	//if fails, it returns nan
		return result;
	}

	void SFilter_Parameter::set_double(const double value, HRESULT &rc) {
		rc = get()->Set_Double(value);
	}

	bool SFilter_Parameter::as_bool(HRESULT &rc) {
		bool result = false;
		uint8_t uint8;
		rc = get()->Get_Bool(&uint8);
		if (rc == S_OK) 
			result = uint8 != 0;
		
		return result;
	}

	void SFilter_Parameter::set_bool(const bool value, HRESULT &rc) {
		rc = get()->Set_Bool(value ? static_cast<uint8_t>(-1) : 0);
	}

	TFilter_Descriptor SFilter_Configuration_Link::descriptor() {
		glucose::TFilter_Descriptor filter_desc = glucose::Null_Filter_Descriptor;

		GUID filter_id;
		if (operator bool())
			if (get()->Get_Filter_Id(&filter_id) == S_OK)
				glucose::get_filter_descriptor_by_id(filter_id, filter_desc);			

		return filter_desc;
	}	

	SPersistent_Filter_Chain_Configuration::SPersistent_Filter_Chain_Configuration() {
		IPersistent_Filter_Chain_Configuration *configuration;
		if (imported::create_persistent_filter_chain_configuration(&configuration) == S_OK)
			reset(configuration, [](IPersistent_Filter_Chain_Configuration* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });							
	}


	void SPersistent_Filter_Chain_Configuration::for_each(std::function<void(glucose::SFilter_Configuration_Link)> callback) {
		glucose::IFilter_Configuration_Link **link_begin, **link_end;
		HRESULT rc = get()->get(&link_begin, &link_end);
		if (rc != S_OK) return;


		for (; *link_begin != *link_end; link_begin++)
			callback(refcnt::make_shared_reference_ext<SFilter_Configuration_Link, IFilter_Configuration_Link>(*link_begin, true));
	}
	
	SFilter_Executor::SFilter_Executor(SPersistent_Filter_Chain_Configuration configuration, glucose::TOn_Filter_Created on_filter_created, const void* on_filter_created_data) {
		glucose::IFilter_Executor *executor;
		if (imported::execute_filter_configuration(configuration.get(), on_filter_created, on_filter_created_data, &executor) == S_OK)
			reset(executor, [](glucose::IFilter_Executor* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}


	HRESULT SFilter_Executor::Execute(glucose::UDevice_Event event) {
		glucose::IDevice_Event *raw_event = event.get();
		event.release();
		return get()->Execute(raw_event);		
	}

	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter) {
		return imported::add_filters(descriptors.data(), descriptors.data() + descriptors.size(), create_filter) == S_OK;
	}

	std::vector<TFilter_Descriptor> get_filter_descriptors() {
		std::vector<TFilter_Descriptor> result;
		TFilter_Descriptor *desc_begin, *desc_end;

		if (imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc) {
		TFilter_Descriptor *desc_begin, *desc_end;

		bool result = imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK;
		if (result) {
			result = false;	//we have to find the filter yet
			for (auto iter = desc_begin; iter != desc_end; iter++)
				if (iter->id == id) {
					//desc = *iter;							assign const won't work with const members and custom operator= will result into undefined behavior as it has const members (so it does not have to be const itself)
					memcpy(&desc, iter, sizeof(decltype(desc)));	//=> memcpy https://stackoverflow.com/questions/9218454/struct-with-const-member
					result = true;
					break;
				}
		}

		return result;
	}


	refcnt::SReferenced<glucose::IFilter_Parameter> SFilter_Configuration::Resolve_Parameter(const wchar_t* name) const {	
		if (!operator bool()) return nullptr;
		
		glucose::IFilter_Parameter **cbegin, **cend;
		if (get()->get(&cbegin, &cend) == S_OK)

			for (glucose::IFilter_Parameter** cur = cbegin; cur < cend; cur++) {
				wchar_t* conf_name;
				if ((*cur)->Get_Config_Name(&conf_name) == S_OK) {
					if (wcscmp(conf_name, name) == 0) {
						return refcnt::make_shared_reference_ext<refcnt::SReferenced<glucose::IFilter_Parameter>, glucose::IFilter_Parameter>(*cur, true);
					}
				}
			}

		return nullptr;	//not found
	}
	

	std::wstring SFilter_Configuration::Read_String(const wchar_t* name, const std::wstring& default_value) const {
		return Read_Parameter<std::wstring>(name, [](refcnt::SReferenced<glucose::IFilter_Parameter> parameter, std::wstring &value) {
			refcnt::wstr_container *wstr;
			HRESULT rc = parameter->Get_WChar_Container(&wstr);
			if (rc == S_OK) {
				value = WChar_Container_To_WString(wstr);
				wstr->Release();
			}
			return rc;
		}, default_value);
	}


	int64_t SFilter_Configuration::Read_Int(const wchar_t* name, const int64_t default_value) const {
		return Read_Parameter<int64_t>(name, [](refcnt::SReferenced<glucose::IFilter_Parameter> parameter, int64_t &value) {
			return parameter->Get_Int64(&value);
		}, default_value);
	}

	std::vector<int64_t> SFilter_Configuration::Read_Int_Array(const wchar_t* name) const {
		const auto parameter = Resolve_Parameter(name);		
		std::vector<int64_t> result;
		if (!parameter) return result;
		
		glucose::time_segment_id_container *ids;
		if (parameter->Get_Time_Segment_Id_Container(&ids) != S_OK) return result;

		result = refcnt::Container_To_Vector<int64_t>(ids);
		ids->Release();
		return result;
	}

	GUID SFilter_Configuration::Read_GUID(const wchar_t* name, const GUID &default_value) const {
		return Read_Parameter<GUID>(name, [](refcnt::SReferenced<glucose::IFilter_Parameter> parameter, GUID &value) {
			return parameter->Get_GUID(&value);
		}, default_value);
	}

	bool SFilter_Configuration::Read_Bool(const wchar_t* name, bool default_value) const {
		return Read_Parameter<bool>(name, [](refcnt::SReferenced<glucose::IFilter_Parameter> parameter, bool &value) {
			uint8_t raw_value;
			HRESULT rc = parameter->Get_Bool(&raw_value);
			if (rc == S_OK) value = raw_value != 0;
			return rc;
		}, default_value);
	}

	double SFilter_Configuration::Read_Double(const wchar_t* name, const double default_value) const {
		return Read_Parameter<double>(name, [](refcnt::SReferenced<glucose::IFilter_Parameter> parameter, double &value) {
			return parameter->Get_Double(&value);
		}, default_value);
	}

	
	void SFilter_Configuration::Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) const {
		const auto parameter = Resolve_Parameter(name);

		bool success = parameter.operator bool();

		if (success) {
			glucose::IModel_Parameter_Vector *raw_parameters;
			if (parameter->Get_Model_Parameters(&raw_parameters) == S_OK) {

				double *begin, *end;
				if (raw_parameters->get(&begin, &end) == S_OK) {				
					if ((begin != nullptr) && (begin != end)) {
						const size_t distance = std::distance(begin, end);
						if (distance % 3 == 0) {
							const size_t paramcnt = distance / 3; // lower, default, upper
							lower_bound = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(begin, &begin[paramcnt]);
							default_parameters = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(&begin[paramcnt], &begin[2 * paramcnt]);
							upper_bound = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(&begin[2 * paramcnt], &begin[3 * paramcnt]);
							success = true;
						}
					}
				}
			}
			raw_parameters->Release();
		}

		if (!success) {
			lower_bound = glucose::SModel_Parameter_Vector{};
			default_parameters = glucose::SModel_Parameter_Vector{};
			upper_bound = glucose::SModel_Parameter_Vector{};
		}

	}


	
	CBase_Filter::CBase_Filter(glucose::IFilter *output) : mOutput(output) {
		//
	}

	CBase_Filter::~CBase_Filter() {
		//

	}

	HRESULT CBase_Filter::Send(glucose::UDevice_Event &event) {
		if (!event) return E_INVALIDARG;

		glucose::IDevice_Event *raw_event = event.get();
		event.release();
		return mOutput->Execute(raw_event);
	}

	
	HRESULT IfaceCalling CBase_Filter::Configure(IFilter_Configuration* configuration) {
		SFilter_Configuration shared_configuration = refcnt::make_shared_reference_ext<SFilter_Configuration, IFilter_Configuration> ( configuration, true);
		return Do_Configure(shared_configuration);
	}

	HRESULT IfaceCalling CBase_Filter::Execute(glucose::IDevice_Event *event) {
		if (!event) return E_INVALIDARG;
		return Do_Execute(glucose::UDevice_Event{event});
	}





	SError_Filter_Inspection::SError_Filter_Inspection(SFilter &error_filter) {
		if (error_filter)
			refcnt::Query_Interface<glucose::IFilter, glucose::IError_Filter_Inspection>(error_filter.get(), Error_Filter_Inspection, *this);
	}

	SDrawing_Filter_Inspection::SDrawing_Filter_Inspection(SFilter &drawing_filter) {
		if (drawing_filter)
			refcnt::Query_Interface<glucose::IFilter, glucose::IDrawing_Filter_Inspection>(drawing_filter.get(), Drawing_Filter_Inspection, *this);
	}

	SLog_Filter_Inspection::SLog_Filter_Inspection(SFilter &log_filter) {
		if (log_filter)
			refcnt::Query_Interface<glucose::IFilter, glucose::ILog_Filter_Inspection>(log_filter.get(), Log_Filter_Inspection, *this);
	}

	bool SLog_Filter_Inspection::pop(std::shared_ptr<refcnt::wstr_list> &list) {
		bool result = false;
		auto ptr_get = get();
		if (ptr_get) {
			refcnt::wstr_list *raw_list;
			if (ptr_get->Pop(&raw_list) == S_OK) {
				list = refcnt::make_shared_reference<refcnt::wstr_list>(raw_list, false);
				result = list.operator bool();
			}
		}

		return result;
	}

	SCalculate_Filter_Inspection::SCalculate_Filter_Inspection(SFilter &calculate_filter) {
		if (calculate_filter)
			refcnt::Query_Interface<glucose::IFilter, glucose::ICalculate_Filter_Inspection>(calculate_filter.get(), Calculate_Filter_Inspection, *this);
	}

}



std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container) {
	int64_t *begin, *end;
	if (container->get(&begin, &end) == S_OK) {
		std::wstring result;
		for (auto iter = begin; iter != end; iter++) {
			if (result.size() > 0) result.append(L" ");

			result.append(std::to_wstring(*iter));
		}
		return result;
	}
	else
		return std::wstring{};
}

glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str) {
	glucose::time_segment_id_container *obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<int64_t>, glucose::time_segment_id_container>(&obj) == S_OK) {
		std::vector<int64_t> ids;

		std::wstringstream value_str(str);
		int64_t value;
		while (value_str >> value)
			ids.push_back(value);
		
		obj->set(ids.data(), ids.data()+ ids.size());
		return obj;
	}
	else
		return nullptr;
}

std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container)
{
	std::wstring result;

	double *begin, *end;
	if (container->get(&begin, &end) == S_OK)
	{
		for (auto iter = begin; iter != end; iter++)
		{
			if (result.size() > 0)
				result += L" ";

			result += std::to_wstring(*iter);
		}
	}

	return result;
}

glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str)
{
	glucose::IModel_Parameter_Vector *obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<double>, glucose::IModel_Parameter_Vector>(&obj) == S_OK)
	{
		std::vector<double> params;

		std::wstringstream value_str(str);
		double value;
		while (value_str >> value)
			params.push_back(value);

		obj->set(params.data(), params.data() + params.size());
		return obj;
	}
	else
		return nullptr;
}
