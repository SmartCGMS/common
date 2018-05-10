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

		#else

			#ifdef _WIN32
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_filter_descriptors(TFilter_Descriptor **begin, TFilter_Descriptor **end);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe);
				extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter);
			#endif
		#endif
	}

	namespace injected
	{
		std::vector<TFilter_Descriptor> addition_filter_descriptors;
		std::vector<TCreate_Filter> create_filter;
	}

	void register_additional_filter(glucose::TGet_Filter_Descriptors get_descriptors, glucose::TCreate_Filter create_filter) {		
		TFilter_Descriptor *desc_begin, *desc_end;
		if (get_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(injected::addition_filter_descriptors));
		}
		
		injected::create_filter.push_back(create_filter);
	}

	std::vector<TFilter_Descriptor> get_filter_descriptors() {
		std::vector<TFilter_Descriptor> result;
		TFilter_Descriptor *desc_begin, *desc_end;

		if (imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		std::copy(injected::addition_filter_descriptors.begin(), injected::addition_filter_descriptors.end(), std::back_inserter(result));

		return result;
	}

	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc) {
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

		if (!result) {			
			for (const auto & iter : injected::addition_filter_descriptors) {
				if (iter.id == id) {
					memcpy(&desc, &iter, sizeof(decltype(desc)));
					result = true;
					break;
				}
			}
		}

		return result;
	}


	SFilter_Pipe create_filter_pipe() {
		SFilter_Pipe result;
		IFilter_Pipe *pipe;
		if (imported::create_filter_pipe(&pipe) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter_Pipe, IFilter_Pipe>(pipe, false);

		return result;
	}

	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output) {
		SFilter result;
		IFilter *filter;

		if (imported::create_filter(&id, input.get(), output.get(), &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);
		else {
			for (auto builder : injected::create_filter)
			{
				if (builder(&id, input.get(), output.get(), &filter) == S_OK)
				{
					result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);
					break;
				}
			}
		}

		return result;
	}

	// increases reference counter on contents due to passing container through pipes;
	// this actually adds the whole filter chain as additional "owner", but since we can't use shared_ptrs in the whole chain,
	// we need to manually add reference and release it later
	// this is needed for shared_ptrs (made from IReferenced) stored locally in filter which created them, because when filter
	// releases this local reference, the reference counter may drop to 0 earlier than expected, and may lead to crash
	void AddRef_Event(TDevice_Event &event) {
		// add references on container objects
		switch (event.event_code) {
			case glucose::NDevice_Event_Code::Information:
			case glucose::NDevice_Event_Code::Warning:
			case glucose::NDevice_Event_Code::Error:			event.info->AddRef();
																break;

			case glucose::NDevice_Event_Code::Parameters:
			case glucose::NDevice_Event_Code::Parameters_Hint:	event.parameters->AddRef();
																break;

			default:											break;
		}
	}

	void Release_Event(TDevice_Event &event) {
		// release references on container objects
		switch (event.event_code) {
			case glucose::NDevice_Event_Code::Information:
			case glucose::NDevice_Event_Code::Warning:
			case glucose::NDevice_Event_Code::Error:			event.info->Release();
																break;

			case glucose::NDevice_Event_Code::Parameters:
			case glucose::NDevice_Event_Code::Parameters_Hint:	event.parameters->Release();
																break;

			default:											break;
		}
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
