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
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
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
#include "scgmsLib.h"
#include "UILib.h"
#include "../utils/string_utils.h"


#include <wchar.h>
#include <iostream>
#include <sstream>
#include "manufactory.h"

namespace scgms {


	namespace imported {
		scgms::TGet_Filter_Descriptors get_filter_descriptors = scgms::factory::resolve_symbol<scgms::TGet_Filter_Descriptors>("get_filter_descriptors");
		scgms::TAdd_Filters add_filters = scgms::factory::resolve_symbol<scgms::TAdd_Filters>("add_filters");
		scgms::TCreate_Persistent_Filter_Chain_Configuration create_persistent_filter_chain_configuration = scgms::factory::resolve_symbol<scgms::TCreate_Persistent_Filter_Chain_Configuration>("create_persistent_filter_chain_configuration");
		scgms::TExecute_Filter_Configuration execute_filter_configuration = scgms::factory::resolve_symbol<scgms::TExecute_Filter_Configuration>("execute_filter_configuration");
		scgms::TCreate_Filter_Parameter create_filter_parameter = scgms::factory::resolve_symbol<scgms::TCreate_Filter_Parameter>("create_filter_parameter");
		scgms::TCreate_Filter_Configuration_Link create_filter_configuration_link = scgms::factory::resolve_symbol<scgms::TCreate_Filter_Configuration_Link>("create_filter_configuration_link");
		scgms::TCreate_Discrete_Model create_discrete_model = scgms::factory::resolve_symbol<scgms::TCreate_Discrete_Model>("create_discrete_model");
	}

	NParameter_Type SFilter_Parameter::type() {
		NParameter_Type result = NParameter_Type::ptNull;
		get()->Get_Type(&result);
		return result;
	}


	const wchar_t* SFilter_Parameter::configuration_name() {
		wchar_t* result = nullptr;
		get()->Get_Config_Name(&result);	
		return result;
	}


	std::wstring SFilter_Parameter::as_wstring(HRESULT &rc, bool read_interpreted) {
		std::wstring result;

		refcnt::wstr_container* wstr;
		rc = get()->Get_WChar_Container(&wstr, read_interpreted);
		if (rc == S_OK) {
			result = refcnt::WChar_Container_To_WString(wstr);
			wstr->Release();
		}

		return result;
	}

	HRESULT SFilter_Parameter::set_wstring(const wchar_t *str) {
		std::shared_ptr<refcnt::wstr_container> wstr = refcnt::WString_To_WChar_Container_shared(str);
		return get()->Set_WChar_Container(wstr.get());
	}

	
	int64_t SFilter_Parameter::as_int(HRESULT &rc) {
		int64_t result = 0;
		rc = get()->Get_Int64(&result);	//if fails, it returns 0
		return result;
	}

	double SFilter_Parameter::as_double(HRESULT &rc) {
		double result = std::numeric_limits<double>::quiet_NaN();
		rc = get()->Get_Double(&result);	//if fails, it returns nan
		return result;
	}

	HRESULT SFilter_Parameter::set_double_array(const std::vector<double> &values) {
		auto parameters = refcnt::Create_Container_shared<double>(const_cast<double*>(values.data()), const_cast<double*>(values.data() + values.size()));
		return get()->Set_Model_Parameters(parameters.get());
	}

	std::vector<double> SFilter_Parameter::as_double_array(HRESULT &rc) {
		std::vector<double> result;
		scgms::IModel_Parameter_Vector *container;
		rc = get()->Get_Model_Parameters(&container);
		if (rc == S_OK) {
			result = refcnt::Container_To_Vector<double>(container);
			container->Release();
		}

		return result;
	}


	HRESULT SFilter_Parameter::double_array_from_wstring(const wchar_t *str_value) {
		HRESULT rc = E_FAIL;
		scgms::IModel_Parameter_Vector *parameters = WString_To_Model_Parameters(str_value);
		if (parameters) {
			rc = get()->Set_Model_Parameters(parameters);
			parameters->Release();
		}

		return rc;
	}


	bool SFilter_Parameter::as_bool(HRESULT &rc) {
		bool result = false;
		BOOL uint8;
		rc = get()->Get_Bool(&uint8);
		if (rc == S_OK) 
			result = uint8 != FALSE;
		
		return result;
	}

	HRESULT SFilter_Parameter::set_bool(const bool value) {
		return get()->Set_Bool(value ? TRUE : FALSE);
	}


	
	std::vector<int64_t> SFilter_Parameter::as_int_array(HRESULT &rc) {
		std::vector<int64_t> result;

		scgms::time_segment_id_container *container;
		rc = get()->Get_Time_Segment_Id_Container(&container);
		if (rc == S_OK) {
			result = refcnt::Container_To_Vector<int64_t>(container);
			container->Release();
		}

		return result;
	}

	HRESULT SFilter_Parameter::set_int_array(const std::vector<int64_t> &values) {
		int64_t *casted_data = const_cast<int64_t*>(values.data());
		refcnt::SVector_Container<int64_t> container = refcnt::Create_Container_shared<int64_t>(casted_data, casted_data + values.size());
		if (container) return get()->Set_Time_Segment_Id_Container(container.get());
			else return E_FAIL;
	}


	HRESULT SFilter_Parameter::int_array_from_wstring(const wchar_t *str_value) {
		HRESULT rc = E_FAIL;
		scgms::time_segment_id_container *container = WString_To_Select_Time_Segments_Id(str_value);
		if (container) {
			rc = get()->Set_Time_Segment_Id_Container(container);
			container->Release();
		}
		
		return rc;
	}

	
	GUID SFilter_Parameter::as_guid(HRESULT &rc) {
		GUID result;
		rc = get()->Get_GUID(&result);
		return result;
	}

	HRESULT SFilter_Parameter::set_GUID(const GUID& guid) {
		return get()->Set_GUID(&guid);
	}


	TFilter_Descriptor SFilter_Configuration_Link::descriptor() {
		scgms::TFilter_Descriptor filter_desc = scgms::Null_Filter_Descriptor;

		GUID filter_id;
		if (operator bool())
			if (get()->Get_Filter_Id(&filter_id) == S_OK)
				scgms::get_filter_descriptor_by_id(filter_id, filter_desc);			

		return filter_desc;
	}	


	SFilter_Parameter SFilter_Configuration_Link::Add_Parameter(const scgms::NParameter_Type type, const wchar_t *conf_name) {
		SFilter_Parameter result;
		scgms::IFilter_Parameter *parameter;
		if (imported::create_filter_parameter(type, conf_name, &parameter) == S_OK) {
			if (get()->add(&parameter, &parameter + 1) == S_OK)
				result = refcnt::make_shared_reference_ext< SFilter_Parameter, IFilter_Parameter>(parameter, false);
		}

		return result;
	}

	scgms::SFilter_Parameter scgms::internal::Create_Filter_Parameter(const scgms::NParameter_Type type, const wchar_t *config_name) {
		scgms::SFilter_Parameter result;
		scgms::IFilter_Parameter *new_parameter;
		if (imported::create_filter_parameter(type, config_name, &new_parameter) == S_OK)
			result = refcnt::make_shared_reference_ext<scgms::SFilter_Parameter, scgms::IFilter_Parameter>(new_parameter, false);
		return result;
	}

	scgms::SFilter_Configuration_Link internal::Create_Configuration_Link(const GUID &id) {
		scgms::SFilter_Configuration_Link result;
		scgms::IFilter_Configuration_Link *link;
		if (imported::create_filter_configuration_link(&id, &link) == S_OK) {
			result = refcnt::make_shared_reference_ext<scgms::SFilter_Configuration_Link, scgms::IFilter_Configuration_Link>(link, false);
		}

		return result;
	}	

	SPersistent_Filter_Chain_Configuration::SPersistent_Filter_Chain_Configuration() {
		IPersistent_Filter_Chain_Configuration *configuration;
		if (imported::create_persistent_filter_chain_configuration(&configuration) == S_OK)
			reset(configuration, [](IPersistent_Filter_Chain_Configuration* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });							
	}


	SFilter_Executor::SFilter_Executor(refcnt::SReferenced<scgms::IFilter_Chain_Configuration> configuration, scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data, refcnt::Swstr_list error_description) {
		scgms::IFilter_Executor *executor;
		if (SUCCEEDED(imported::execute_filter_configuration(configuration.get(), on_filter_created, on_filter_created_data, nullptr, &executor, error_description.get())))
			reset(executor, [](scgms::IFilter_Executor* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}


	HRESULT SFilter_Executor::Execute(scgms::UDevice_Event &&event) {
		scgms::IDevice_Event *raw_event = event.get();
		event.release();
		return get()->Execute(raw_event);
	}

	bool add_filters(const std::vector<scgms::TFilter_Descriptor> &descriptors, scgms::TCreate_Filter create_filter) {
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

	
	CBase_Filter::CBase_Filter(scgms::IFilter* output, const GUID& device_id) : mOutput(output), mDevice_ID(device_id) {
		if (!mOutput) 
			throw std::runtime_error{ "Attempted to construct a filter without valid output!" };
	}

	CBase_Filter::~CBase_Filter() {
		//

	}


	HRESULT CBase_Filter::Send(scgms::UDevice_Event &event) {
		if (!event) return E_INVALIDARG;

		scgms::IDevice_Event *raw_event = event.get();
		event.release();
		return mOutput->Execute(raw_event);
	}

	
	void CBase_Filter::Emit_Info(const scgms::NDevice_Event_Code code, const std::wstring& msg, const uint64_t segment_id) {
		scgms::UDevice_Event event{ code };
		event.device_id() = mDevice_ID;
		event.info.set(msg.c_str());
		event.segment_id() = segment_id;
		Send(event);
	}

	HRESULT IfaceCalling CBase_Filter::Configure(IFilter_Configuration* configuration, refcnt::wstr_list* error_description) {
		SFilter_Configuration shared_configuration = refcnt::make_shared_reference_ext<SFilter_Configuration, IFilter_Configuration> ( configuration, true);		
		refcnt::Swstr_list shared_error_description = refcnt::make_shared_reference_ext<refcnt::Swstr_list, refcnt::wstr_list>(error_description, true);
		return Do_Configure(shared_configuration, shared_error_description);
	}

	HRESULT IfaceCalling CBase_Filter::Execute(scgms::IDevice_Event *event) {
		if (!event) return E_INVALIDARG;
		return Do_Execute(scgms::UDevice_Event{event});
	}

	SDiscrete_Model::SDiscrete_Model() : refcnt::SReferenced<scgms::IDiscrete_Model>(){
		//
	}

	SDiscrete_Model::SDiscrete_Model(const GUID &id, const std::vector<double> &parameters, scgms::SFilter output) {
		scgms::SModel_Parameter_Vector parameters_shared = refcnt::Create_Container_shared<double, scgms::SModel_Parameter_Vector>(const_cast<double*>(parameters.data()), const_cast<double*>(parameters.data()+parameters.size()));			

		scgms::IDiscrete_Model *model;
		if (imported::create_discrete_model(&id, parameters_shared.get(), output.get(), &model) == S_OK)
			reset(model, [](scgms::IDiscrete_Model* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}
	
	SDrawing_Filter_Inspection::SDrawing_Filter_Inspection(const SFilter &drawing_filter) {
		if (drawing_filter)
			refcnt::Query_Interface<scgms::IFilter, scgms::IDrawing_Filter_Inspection>(drawing_filter.get(), IID_Drawing_Filter_Inspection, *this);
	}

	SLog_Filter_Inspection::SLog_Filter_Inspection(const SFilter &log_filter) {
		if (log_filter)
			refcnt::Query_Interface<scgms::IFilter, scgms::ILog_Filter_Inspection>(log_filter.get(), IID_Log_Filter_Inspection, *this);
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

	SSignal_Error_Inspection::SSignal_Error_Inspection(const SFilter &signal_error_filter) {
		if (signal_error_filter)
			refcnt::Query_Interface<scgms::IFilter, scgms::ISignal_Error_Inspection>(signal_error_filter.get(), IID_Signal_Error_Inspection, *this);
	}

}



std::wstring Select_Time_Segments_Id_To_WString(scgms::time_segment_id_container *container) {
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

scgms::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str) {
	if (!str) return nullptr;

	std::vector<int64_t> ids;
	std::wstring str_copy{ str };	//wcstok modifies the input string
	const wchar_t* delimiters = L" ";	//string of chars, which designate individual delimiters
	wchar_t* buffer = nullptr;
	wchar_t* str_val = wcstok_s(str_copy.data(), delimiters, &buffer);
	while (str_val != nullptr) {
		bool ok;
		const int64_t value = wstr_2_int(str_val, ok);
		if (!ok) return nullptr;
		ids.push_back(value);

		str_val = wcstok_s(nullptr, delimiters, &buffer);
	}


	scgms::time_segment_id_container *obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<int64_t>, scgms::time_segment_id_container>(&obj) == S_OK) {		
		if (obj->set(ids.data(), ids.data() + ids.size()) != S_OK) {
			obj->Release();
			obj = nullptr;
		};		
	}
	
	return obj;
}

std::wstring Model_Parameters_To_WString(scgms::IModel_Parameter_Vector *container) {
	
	std::wstringstream result;
	//let's declare dec_sep as a named var to avoid a static-analysis warning
	//=>refs must be 1, other locale would free it upon passing it
	CDecimal_Separator<wchar_t>* decimal_separator = new CDecimal_Separator<wchar_t>{L'.', 1};	
	result.imbue(std::locale(std::wcout.getloc(), decimal_separator)); //locale takes owner ship of dec_sep

	bool not_empty = false;

	double *begin, *end;
	if (container->get(&begin, &end) == S_OK)
	{
		for (auto iter = begin; iter != end; iter++)
		{
			if (not_empty)
				result << L" ";
			else
				not_empty = true;

			result << *iter;
		}
	}

	return result.str();
}

scgms::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str) {
	if (!str) return nullptr;

	std::vector<double> params;
	std::wstring str_copy{ str };	//wcstok modifies the input string
	const wchar_t* delimiters = L" ";	//string of chars, which designate individual delimiters
	wchar_t* buffer = nullptr;
	wchar_t* str_val = wcstok_s(str_copy.data(), delimiters, &buffer);
	while (str_val != nullptr) {
		bool ok;
		const double value = wstr_2_dbl(str_val, ok);
		if (!ok) return nullptr;
		params.push_back(value);

		str_val = wcstok_s(nullptr, delimiters, &buffer);
	}



	scgms::IModel_Parameter_Vector* obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<double>, scgms::IModel_Parameter_Vector>(&obj) == S_OK) {
		if (obj->set(params.data(), params.data() + params.size()) != S_OK) {
			obj->Release();
			obj = nullptr;
		};
	}

	return obj;
}
