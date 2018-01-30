#include "FilterLib.h"



namespace glucose {


	namespace imported {

		#ifdef _WIN32
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_filter_descriptors(TFilter_Descriptor **begin, TFilter_Descriptor **end);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter);
		#endif
	}


	std::vector<TFilter_Descriptor> get_filter_descriptors() {
		std::vector<TFilter_Descriptor> result;
		TFilter_Descriptor *desc_begin, *desc_end;

		if (imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		return result;
	}

	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc) {
		TFilter_Descriptor *desc_begin, *desc_end;

		bool result = imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK;
		if (result) {
			result = false;	//we have to find the filter yet
			for (auto iter=desc_begin; iter != desc_end; iter++)
				if (iter->id == id) {
					desc = *iter;
					result = true;
					break;
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

		return result;
	}

	


}

std::wstring WChar_Container_To_WString(glucose::wstr_contrainer *container) {
	wchar_t *begin, *end;
	if (container->get(&begin, &end)) {
		return std::wstring{ begin, end };
	}
	else
		return std::wstring{};
}

glucose::wstr_contrainer* WString_To_WChar_Container(const wchar_t* str) {
	glucose::IParameter_Container<wchar_t> *obj = nullptr;
	if (Manufacture_Object<glucose::internal::CParameter_Container<wchar_t>, glucose::IParameter_Container<wchar_t>>(&obj) == S_OK) {
		obj->set(str, str + wcslen(str));
		return obj;
	}
	else return
		nullptr;
	
}