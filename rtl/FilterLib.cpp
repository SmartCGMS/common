/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#include "FilterLib.h"

#include "FactoryLib.h"

#include <sstream>
#include "manufactory.h"

namespace glucose {


	namespace imported {
		glucose::TGet_Filter_Descriptors get_filter_descriptors = factory::resolve_symbol<glucose::TGet_Filter_Descriptors>("get_filter_descriptors");
		glucose::TCreate_Filter_Pipe create_filter_pipe = factory::resolve_symbol<glucose::TCreate_Filter_Pipe>("create_filter_pipe");
		glucose::TCreate_Filter create_filter = factory::resolve_symbol<glucose::TCreate_Filter>("create_filter");
		glucose::TAdd_Filters add_filters = factory::resolve_symbol<glucose::TAdd_Filters>("add_filters");
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

	
	bool SFilter_Pipe::Send(UDevice_Event &event) {
		if (!operator bool()) return false;
		if (!event) return false;
		
		if (get()->send(event.get()) != S_OK) {
			event.reset();	//delete and release the event anyway to prevent the event from being deleted twice
			return false;
		}
			
		event.release(); 	//release the resource from the sender, but do not delete it to let it live in the next filter
		
		return true;
	}

	UDevice_Event SFilter_Pipe::Receive() {
		if (!operator bool()) return UDevice_Event{};

		IDevice_Event *raw_event;
		if (get()->receive(&raw_event) != S_OK) return nullptr;
		
		return UDevice_Event{ raw_event };
	}


	SFilter_Pipe::SFilter_Pipe(glucose::IFilter_Pipe *pipe) {
		if (pipe) pipe->AddRef();
		reset(pipe, [](glucose::IFilter_Pipe* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}

	SFilter_Pipe::SFilter_Pipe() {
		
		IFilter_Pipe *pipe;
		if (imported::create_filter_pipe(&pipe) == S_OK)
			reset(pipe, [](glucose::IFilter_Pipe* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });				
	}

	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output) {
		SFilter result;
		IFilter *filter;

		if (imported::create_filter(&id, input.get(), output.get(), &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);
	
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

	TFilter_Parameter* SFilter_Parameters::Resolve_Parameter(const wchar_t* name) {
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

	std::wstring SFilter_Parameters::Read_String(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? WChar_Container_To_WString(parameter->wstr) : std::wstring{};
	}


	int64_t SFilter_Parameters::Read_Int(const wchar_t* name, const int64_t default_value) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->int64 : default_value;
	}

	std::vector<int64_t> SFilter_Parameters::Read_Int_Array(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);

		std::vector<int64_t> result;

		if (parameter)
			result = refcnt::Container_To_Vector<int64_t>(parameter->select_time_segment_id);		

		return result;
	}

	GUID SFilter_Parameters::Read_GUID(const wchar_t* name, const GUID &default_value) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->guid : default_value;
	}

	bool SFilter_Parameters::Read_Bool(const wchar_t* name, bool default_value) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->boolean : default_value;
	}

	double SFilter_Parameters::Read_Double(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->dbl : std::numeric_limits<double>::quiet_NaN();
	}

	std::vector<double> SFilter_Parameters::Read_Double_Array(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);

		std::vector<double> result;

		if (parameter)
			result = refcnt::Container_To_Vector<double>(parameter->parameters);

		return result;
	}

	void SFilter_Parameters::Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) {
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
