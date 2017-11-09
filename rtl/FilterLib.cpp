#include "FilterLib.h"

#include "referencedImpl.h"

namespace glucose {

	wchar_t* SFilter_Factory::description() const {
		wchar_t *result;

		auto obj = get();
		if (obj && obj->description(&result) == S_OK) return result;
			else return nullptr;
	}


	SFilter SFilter_Factory::create_filter(SFilter_Pipe &input, SFilter_Pipe &output) {
		SFilter result;
		IFilter *filter;
		
		auto obj = get();
		if (obj && obj->create_filter(input.get(), output.get(), &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);

		return result;
	}


	namespace imported {

		#ifdef _WIN32
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_filter_factories(IFilter_Factory **begin, IFilter_Factory **end);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe);
		#endif
	}


	std::vector<SFilter_Factory> get_filter_factories() {
		std::vector<SFilter_Factory> result;
		IFilter_Factory *iter, *end;

		if (imported::get_filter_factories(&iter, &end) == S_OK) {
			while (iter != end) {

				result.push_back(refcnt::make_shared_reference_ext<SFilter_Factory, IFilter_Factory>(iter, true));

				iter++;
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

}