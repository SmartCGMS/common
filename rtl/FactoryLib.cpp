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

#include "FactoryLib.h"

namespace factory {

	namespace internal {

		const wchar_t *factory_dynamic_lib_name = L"factory";

	#ifdef _WIN32
			const wchar_t* rsShared_Object_Prefix = L"";
			const wchar_t* rsShared_Object_Extension = L".dll";
	#elif __APPLE__
			const wchar_t* rsShared_Object_Prefix = L"lib";
			const wchar_t* rsShared_Object_Extension = L".dylib";
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
			if (strcmp(symbol_name, "create_signal") == 0) return reinterpret_cast<void(*)>(internal::create_signal_not_impl);
			if (strcmp(symbol_name, "create_device_event") == 0) return reinterpret_cast<void(*)>(internal::create_device_event_not_impl);
			if (strcmp(symbol_name, "create_filter_pipe") == 0) return reinterpret_cast<void(*)>(internal::create_filter_pipe_not_impl);
			if (strcmp(symbol_name, "create_filter") == 0) return reinterpret_cast<void(*)>(internal::create_filter_not_impl);
			if (strcmp(symbol_name, "add_filters") == 0) return reinterpret_cast<void(*)>(internal::add_filters_not_impl);
			if (strcmp(symbol_name, "create_approximator") == 0) return reinterpret_cast<void(*)>(internal::create_approximator_not_impl);
			if (strcmp(symbol_name, "solve_model_parameters") == 0) return reinterpret_cast<void(*)>(internal::solve_model_parameters_not_impl);


			return reinterpret_cast<void(*)>(internal::get_desc_not_impl);
		}

	}

}