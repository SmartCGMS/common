#include "FilterLib.h"

#include <sstream>
#include "manufactory.h"

namespace glucose {


	namespace imported {

		//#define DIMPORT_TEST_FAIL E_NOTIMPL

		#ifdef DIMPORT_TEST_FAIL
			HRESULT IfaceCalling get_filter_descriptors(TFilter_Descriptor **begin, TFilter_Descriptor **end) {
				return DIMPORT_TEST_FAIL;
			}

			HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe) {
				return DIMPORT_TEST_FAIL;
			}

			HRESULT IfaceCalling create_filter(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter) {
				return DIMPORT_TEST_FAIL;
			}

			HRESULT IfaceCalling add_filters(const glucose::TFilter_Descriptor *begin, const glucose::TFilter_Descriptor *end, const glucose::TCreate_Filter create_filter) {
				return DIMPORT_TEST_FAIL;
			}

		#else

			#ifdef _WIN32
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_filter_descriptors(TFilter_Descriptor **begin, TFilter_Descriptor **end);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling add_filters(const glucose::TFilter_Descriptor *begin, const glucose::TFilter_Descriptor *end, const glucose::TCreate_Filter create_filter);
			#endif
		#endif
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
		
		if (get()->send(event.get()) != S_OK) return false;
			
		event.release(); 	//release the resource from the sender
		
		return true;
	}

	UDevice_Event SFilter_Pipe::Receive() {
		if (!operator bool()) return false;

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

	

	void Release_Filter_Parameter(TFilter_Parameter &parameter) {
		if (parameter.config_name) parameter.config_name->Release();

		switch (parameter.type) {
		case glucose::NParameter_Type::ptWChar_Container: parameter.wstr->Release();
			parameter.wstr = nullptr;
			break;
		case glucose::NParameter_Type::ptSelect_Time_Segment_ID: parameter.select_time_segment_id->Release();
			parameter.select_time_segment_id = nullptr;
			break;

		case glucose::NParameter_Type::ptModel_Bounds: parameter.parameters->Release();
			parameter.parameters = nullptr;
			break;
		}
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


	int64_t SFilter_Parameters::Read_Int(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->int64 : std::numeric_limits<int64_t>::max();
	}

	std::vector<int64_t> SFilter_Parameters::Read_Int_Array(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);

		std::vector<int64_t> result;

		if (parameter)
			result = refcnt::Container_To_Vector<int64_t>(parameter->select_time_segment_id);		

		return result;
	}

	GUID SFilter_Parameters::Read_GUID(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->guid : Invalid_GUID;
	}

	bool SFilter_Parameters::Read_Bool(const wchar_t* name) {
		const auto parameter = Resolve_Parameter(name);
		return parameter != nullptr ? parameter->boolean : false;
	}

	SError_Filter_Inspection::SError_Filter_Inspection(SFilter &error_filter) {
		refcnt::Query_Interface<glucose::IFilter, glucose::IError_Filter_Inspection>(error_filter.get(), Error_Filter_Inspection, *this);		
	}

	SDrawing_Filter_Inspection::SDrawing_Filter_Inspection(SFilter &drawing_filter) {
		refcnt::Query_Interface<glucose::IFilter, glucose::IDrawing_Filter_Inspection>(drawing_filter.get(), Drawing_Filter_Inspection, *this);
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
