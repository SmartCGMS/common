#include "FactoryLib.h"

namespace factory {

	namespace internal {

		const wchar_t *factory_dynamic_lib_name = L"factory";

	#ifdef _WIN32
			const wchar_t* rsShared_Object_Prefix = L"";
			const wchar_t* rsShared_Object_Extension = L".dll";
	#else
			const wchar_t* rsShared_Object_Prefix = L"lib";
			const wchar_t* rsShared_Object_Extension = L".so";
	#endif 

			std::wstring Get_Platform_Library_Name(const wchar_t* library_name) {
				std::wstring name = rsShared_Object_Prefix;
				name += library_name;
				name += rsShared_Object_Extension;

				return name;
			}

		CDynamic_Library gFactory_Library{};

		void* resolve_factory_symbol(const char* symbol_name) {
			if (!gFactory_Library.Is_Loaded()) {
				std::wstring path = Get_Platform_Library_Name(factory_dynamic_lib_name);
		
				if (!gFactory_Library.Load(path.c_str())) {
					return nullptr;
				}
			}

			return gFactory_Library.Resolve(symbol_name);
		}

		//stub for:
		//	get_metric_descriptors
		//	get_model_descriptors
		//	get_solver_descriptors
		//	get_filter_descriptors
		//	create_metric
		HRESULT IfaceCalling get_desc_not_impl(void *begin, void *end) { return E_NOTIMPL; }
		//specialized stubs
		HRESULT IfaceCalling create_signal_not_impl(void *signal_id, void* *segment, void *signal) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_device_event_not_impl(void *event, glucose::NDevice_Event_Code code) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_filter_pipe_not_impl(void* pipe) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_filter_not_impl(void* id, void* input, void* output, void* filter) { return E_NOTIMPL; }
		HRESULT IfaceCalling add_filters_not_impl(void *begin, void *end, void* create_filter) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_approximator_not_impl(void* approx_id, void *signal, void* configuration, void* approx) { return E_NOTIMPL; }
		HRESULT IfaceCalling solve_model_parameters_not_impl(void *setup) { return E_NOTIMPL; }
		

		void* resolve_not_impl_symbol(const char* symbol_name) {
			if (strcmp(symbol_name, "create_signal") == 0) return internal::create_signal_not_impl;
			if (strcmp(symbol_name, "create_device_event") == 0) return internal::create_device_event_not_impl;
			if (strcmp(symbol_name, "create_filter_pipe") == 0) return internal::create_filter_pipe_not_impl;
			if (strcmp(symbol_name, "create_filter") == 0) return internal::create_filter_not_impl;
			if (strcmp(symbol_name, "add_filters") == 0) return internal::add_filters_not_impl;
			if (strcmp(symbol_name, "create_approximator") == 0) return internal::create_approximator_not_impl;
			if (strcmp(symbol_name, "solve_model_parameters") == 0) return internal::solve_model_parameters_not_impl;


			return internal::get_desc_not_impl;
		}

	}

}