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
		glucose::TCreate_Filter_Asynchronous_Pipe create_filter_asynchronous_pipe = factory::resolve_symbol<glucose::TCreate_Filter_Asynchronous_Pipe>("create_filter_asynchronous_pipe");
		glucose::TCreate_Filter_Synchronous_Pipe create_filter_synchronous_pipe = factory::resolve_symbol<glucose::TCreate_Filter_Synchronous_Pipe>("create_filter_synchronous_pipe");
		glucose::TCreate_Asynchronous_Filter create_asynchronous_filter = factory::resolve_symbol<glucose::TCreate_Asynchronous_Filter>("create_asynchronous_filter");
		glucose::TCreate_Synchronous_Filter create_synchronous_filter = factory::resolve_symbol<glucose::TCreate_Synchronous_Filter>("create_synchronous_filter");
		glucose::TAdd_Filters add_filters = factory::resolve_symbol<glucose::TAdd_Filters>("add_filters");
	}


	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Asynchronous_Filter create_asynchronous_filter, glucose::TCreate_Synchronous_Filter create_synchronous_filter) {
		return imported::add_filters(descriptors.data(), descriptors.data() + descriptors.size(), create_asynchronous_filter, create_synchronous_filter) == S_OK;
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


	SAsynchronous_Filter create_asynchronous_filter(const GUID &id, glucose::IFilter_Asynchronous_Pipe *input, glucose::IFilter_Asynchronous_Pipe *output) {
		SAsynchronous_Filter result;
		IAsynchronous_Filter *filter;

		if (imported::create_asynchronous_filter(&id, input, output, &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SAsynchronous_Filter, IAsynchronous_Filter>(filter, false);

		return result;
	}

	SSynchronous_Filter create_synchronous_filter(const GUID &id) {
		SSynchronous_Filter result;
		ISynchronous_Filter *filter;

		if (imported::create_synchronous_filter(&id, &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SSynchronous_Filter, ISynchronous_Filter>(filter, false);

		return result;
	}


	void Visit_Filter_Parameter(glucose::TFilter_Parameter& element, std::function<void(refcnt::IReferenced *obj)> func) {
		if (element.config_name != nullptr) func(element.config_name);

		switch (element.type) {
			case glucose::NParameter_Type::ptWChar_Container: if (element.wstr != nullptr) func(element.wstr);
				break;
			case glucose::NParameter_Type::ptSelect_Time_Segment_ID: if (element.select_time_segment_id != nullptr) func(element.select_time_segment_id);
				break;
			case glucose::NParameter_Type::ptModel_Bounds: if (element.parameters != nullptr) func(element.parameters);
				break;
			default:
				break;
		}
	}

	void Release_Filter_Parameter(TFilter_Parameter &parameter) {
		Visit_Filter_Parameter(parameter, [](refcnt::IReferenced *obj) { obj->Release(); });
	}

	TFilter_Parameter* SFilter_Parameters::Resolve_Parameter(const wchar_t* name) const {
		if (!operator bool()) return nullptr;

		TFilter_Parameter* result = nullptr;
		glucose::TFilter_Parameter *cbegin, *cend;
		if (get()->get(&cbegin, &cend) == S_OK)
			for (glucose::TFilter_Parameter* cur = cbegin; cur < cend; cur++)
				if (WChar_Container_Equals_WString(cur->config_name, name)) {
					result = cur;
					break;
				}
		return result;
	}

	std::wstring SFilter_Parameters::Read_String(const wchar_t* name, const std::wstring& default_value) const {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? WChar_Container_To_WString(parameter->wstr) : default_value;
	}


	int64_t SFilter_Parameters::Read_Int(const wchar_t* name, const int64_t default_value) const {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->int64 : default_value;
	}

	std::vector<int64_t> SFilter_Parameters::Read_Int_Array(const wchar_t* name) const {
		const auto parameter = Resolve_Parameter(name);

		std::vector<int64_t> result;

		if (parameter)
			result = refcnt::Container_To_Vector<int64_t>(parameter->select_time_segment_id);

		return result;
	}

	GUID SFilter_Parameters::Read_GUID(const wchar_t* name, const GUID &default_value) const {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->guid : default_value;
	}

	bool SFilter_Parameters::Read_Bool(const wchar_t* name, bool default_value) const {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->boolean : default_value;
	}

	double SFilter_Parameters::Read_Double(const wchar_t* name, const double default_value) const {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->dbl : default_value;
	}

	std::vector<double> SFilter_Parameters::Read_Double_Array(const wchar_t* name) const {
		const auto parameter = Resolve_Parameter(name);

		std::vector<double> result;

		if (parameter)
			result = refcnt::Container_To_Vector<double>(parameter->parameters);

		return result;
	}

	void SFilter_Parameters::Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) const {
		const auto parameter = Resolve_Parameter(name);

		bool success = parameter != nullptr;
		double *begin, *end;

		if (success) {
			if (parameter->parameters->get(&begin, &end) == S_OK) {
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

		if (!success) {
			lower_bound = glucose::SModel_Parameter_Vector{};
			default_parameters = glucose::SModel_Parameter_Vector{};
			upper_bound = glucose::SModel_Parameter_Vector{};
		}

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

	SDevice_Event_Vector::SDevice_Event_Vector() {
		reset(refcnt::Create_Container<glucose::IDevice_Event*>(nullptr, nullptr),
			[](glucose::IDevice_Event_Vector* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); }
		);
	}

	SDevice_Event_Vector::~SDevice_Event_Vector() {
		if (!mAddedEvents.empty()) {
			// should we warn the user? This may be a result of forgotten Apply call
			discard_push();
		}
	}

	bool SDevice_Event_Vector::push(glucose::UDevice_Event& evt) {
		if (!operator bool()) return false;
		if (!evt) return false;

		glucose::IDevice_Event* raw = evt.get();

		get()->add(&raw, &raw + 1);

		evt.release();
		return true;
	}

	bool SDevice_Event_Vector::push_deferred(glucose::UDevice_Event& evt) {
		if (!operator bool()) return false;
		if (!evt) return false;

		glucose::IDevice_Event* raw = evt.get();

		mAddedEvents.push_back(raw);

		evt.release();
		return true;
	}

	bool SDevice_Event_Vector::commit_push() {
		if (get()->add(mAddedEvents.data(), mAddedEvents.data() + mAddedEvents.size()) != S_OK) {
			discard_push();
			return false;
		}

		mAddedEvents.clear();

		return true;
	}

	bool SDevice_Event_Vector::discard_push() {
		// since we dropped reference counting during Add call, we need to explicitly release the reference now
		for (auto& evt : mAddedEvents)
			evt->Release();

		mAddedEvents.clear();
		return true;
	}

	SFilter_Asynchronous_Pipe::SFilter_Asynchronous_Pipe() : SFilter_Pipe() {
		IFilter_Asynchronous_Pipe *pipe;
		if (imported::create_filter_asynchronous_pipe(&pipe) == S_OK)
			reset(pipe, [](IFilter_Asynchronous_Pipe* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}
		
	bool SFilter_Asynchronous_Pipe::add_filter(SSynchronous_Filter &filter) {
		return false;
	}

	SFilter_Synchronous_Pipe::SFilter_Synchronous_Pipe() : SFilter_Pipe() {
		IFilter_Synchronous_Pipe *pipe;
		if (imported::create_filter_synchronous_pipe(&pipe) == S_OK)
			reset(pipe, [](IFilter_Synchronous_Pipe* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}	

	bool SFilter_Synchronous_Pipe::add_filter(SSynchronous_Filter &filter) {
		auto self = get();
		if (!self) return false;
		return self->add_filter(filter.get()) == S_OK;
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
