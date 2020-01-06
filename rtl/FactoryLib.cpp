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

#include "FactoryLib.h"
#include "FilesystemLib.h"

namespace factory {

	namespace internal {

		const wchar_t *factory_dynamic_lib_name = L"factory";

	#ifdef _WIN32
			const wchar_t* rsShared_Object_Prefix = L"";
			const wchar_t* rsShared_Object_Extension = L".dll";
	#elif __APPLE__
			const wchar_t* rsShared_Object_Prefix = L"lib";
			const wchar_t* rsShared_Object_Extension = L".dylib";
	#elif defined(__ARM_ARCH_7A__) || defined(__aarch64__)
			const wchar_t* rsShared_Object_Prefix = L"libgpredict3_mobile_";
			const wchar_t* rsShared_Object_Extension = L"_interop.so";
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

		CDynamic_Library gFactory_Library;

		void* resolve_factory_symbol(const char* symbol_name) {			

			if (!gFactory_Library.Is_Loaded()) {
				std::wstring path = Get_Platform_Library_Name(factory_dynamic_lib_name);
		
				if (!gFactory_Library.Load(path.c_str())) {
					return nullptr;
				}
			}

			return gFactory_Library.Resolve(symbol_name);
		}

		const char* rsGet_Filter_Descriptors = "get_filter_descriptors";
		const char* rsGet_Model_Descriptors = "get_model_descriptors";
		const char* rsGet_Metric_Descriptors = "get_metric_descriptors";
		const char* rsGet_Solver_Descriptors = "get_solver_descriptors";
		const char* rsGet_Approximator_Descriptors = "get_approximator_descriptors";
		const char* rsGet_Device_Driver_Descriptors = "get_device_driver_descriptors";
		const char* rsCreate_Signal = "create_signal";
		const char* rsCreate_Device_Event = "create_device_event";
		const char* rsCreate_Filter_Asynchronous_Pipe = "create_filter_asynhronous_pipe";
		const char* rsCreate_Filter = "create_filter";		
		const char* rsCreate_Device_Driver = "create_device_driver";
		const char* rsAdd_Filters = "add_filters";
		const char* rsCreate_Approximator = "create_approximator";
		const char* rsSolve_Model_Parameters = "solve_model_parameters";


		//stub for:
		//	get_metric_descriptors
		//	get_model_descriptors
		//	get_solver_descriptors
		//	get_filter_descriptors
		//	get_pump_driver_descriptors
		//	get_sensor_driver_descriptors
		//	create_metric
		HRESULT IfaceCalling get_desc_not_impl(void *begin, void *end) { return E_NOTIMPL; }
		//specialized stubs
		HRESULT IfaceCalling create_signal_not_impl(void *signal_id, void* *segment, void *signal) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_device_event_not_impl(void *event, scgms::NDevice_Event_Code code) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_filter_pipe_not_impl(void* pipe) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_filter_not_impl(void* id, void* input, void* output, void* filter) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_device_driver_not_impl(void* id, void* pump_driver) { return E_NOTIMPL; }
		HRESULT IfaceCalling add_filters_not_impl(void *begin, void *end, void* create_filter) { return E_NOTIMPL; }
		HRESULT IfaceCalling create_approximator_not_impl(void* approx_id, void *signal, void* configuration, void* approx) { return E_NOTIMPL; }
		HRESULT IfaceCalling solve_model_parameters_not_impl(void *setup) { return E_NOTIMPL; }

		template<typename... Args>
		HRESULT factory_lazy_load(const char* symbol_name, Args... args)
		{
			void* resolved = resolve_factory_symbol(symbol_name);
			if (!resolved)
				return E_NOTIMPL;

			return reinterpret_cast<HRESULT(*)(Args...)>(resolved)(args...);
		}

		// lazy loading routines
		HRESULT IfaceCalling get_filter_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Filter_Descriptors, begin, end); }
		HRESULT IfaceCalling get_model_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Model_Descriptors, begin, end); }
		HRESULT IfaceCalling get_metric_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Metric_Descriptors, begin, end); }
		HRESULT IfaceCalling get_solver_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Solver_Descriptors, begin, end); }
		HRESULT IfaceCalling get_approximator_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Approximator_Descriptors, begin, end); }
		HRESULT IfaceCalling get_device_driver_desc_lazy(void *begin, void *end) { return factory_lazy_load(rsGet_Device_Driver_Descriptors, begin, end); }
		HRESULT IfaceCalling create_signal_lazy(void *signal_id, void* *segment, void *signal) { return factory_lazy_load(rsCreate_Signal, signal_id, segment, signal); }
		HRESULT IfaceCalling create_device_event_lazy(void *event, scgms::NDevice_Event_Code code) { return factory_lazy_load(rsCreate_Device_Event, event, code); }
		HRESULT IfaceCalling create_filter_pipe_lazy(void* pipe) { return factory_lazy_load(rsCreate_Filter_Asynchronous_Pipe, pipe); }
		HRESULT IfaceCalling create_filter_lazy(void* id, void* input, void* output, void* filter) { return factory_lazy_load(rsCreate_Filter, id, input, output, filter); }		
		HRESULT IfaceCalling create_device_driver_lazy(void* id, void* pump_driver) { return factory_lazy_load(rsCreate_Device_Driver, id, pump_driver); }
		HRESULT IfaceCalling add_filters_lazy(void *begin, void *end, void* create_filter) { return factory_lazy_load(rsAdd_Filters, begin, end, create_filter); }
		HRESULT IfaceCalling create_approximator_lazy(void* approx_id, void *signal, void* configuration, void* approx) { return factory_lazy_load(rsCreate_Approximator, approx_id, signal, configuration, approx); }
		HRESULT IfaceCalling solve_model_parameters_lazy(void *setup) { return factory_lazy_load(rsSolve_Model_Parameters, setup); }
		

		void* resolve_not_impl_symbol(const char* symbol_name) {
#if defined(__ARM_ARCH_7A__) || defined(__aarch64__)
			if (strcmp(symbol_name, rsGet_Filter_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_filter_desc_lazy);
			if (strcmp(symbol_name, rsGet_Model_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_model_desc_lazy);
			if (strcmp(symbol_name, rsGet_Metric_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_metric_desc_lazy);
			if (strcmp(symbol_name, rsGet_Solver_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_solver_desc_lazy);
			if (strcmp(symbol_name, rsGet_Approximator_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_approximator_desc_lazy);
			if (strcmp(symbol_name, rsGet_Device_Driver_Descriptors) == 0) return reinterpret_cast<void(*)>(internal::get_device_driver_desc_lazy);

			if (strcmp(symbol_name, rsCreate_Signal) == 0) return reinterpret_cast<void(*)>(internal::create_signal_lazy);
			if (strcmp(symbol_name, rsCreate_Device_Event) == 0) return reinterpret_cast<void(*)>(internal::create_device_event_lazy);
			if (strcmp(symbol_name, rsCreate_Filter_Pipe) == 0) return reinterpret_cast<void(*)>(internal::create_filter_pipe_lazy);
			if (strcmp(symbol_name, rsCreate_Filter) == 0) return reinterpret_cast<void(*)>(internal::create_filter_lazy);
			if (strcmp(symbol_name, rsCreate_Device_Driver) == 0) return reinterpret_cast<void(*)>(internal::create_device_driver_lazy);
			if (strcmp(symbol_name, rsAdd_Filters) == 0) return reinterpret_cast<void(*)>(internal::add_filters_lazy);
			if (strcmp(symbol_name, rsCreate_Approximator) == 0) return reinterpret_cast<void(*)>(internal::create_approximator_lazy);
			if (strcmp(symbol_name, rsSolve_Model_Parameters) == 0) return reinterpret_cast<void(*)>(internal::solve_model_parameters_lazy);
#else
			if (strcmp(symbol_name, rsCreate_Signal) == 0) return reinterpret_cast<void(*)>(internal::create_signal_not_impl);
			if (strcmp(symbol_name, rsCreate_Device_Event) == 0) return reinterpret_cast<void(*)>(internal::create_device_event_not_impl);
			if (strcmp(symbol_name, rsCreate_Filter_Asynchronous_Pipe) == 0) return reinterpret_cast<void(*)>(internal::create_filter_pipe_not_impl);
			if (strcmp(symbol_name, rsCreate_Filter) == 0) return reinterpret_cast<void(*)>(internal::create_filter_not_impl);
			if (strcmp(symbol_name, rsCreate_Device_Driver) == 0) return reinterpret_cast<void(*)>(internal::create_device_driver_not_impl);
			if (strcmp(symbol_name, rsAdd_Filters) == 0) return reinterpret_cast<void(*)>(internal::add_filters_not_impl);
			if (strcmp(symbol_name, rsCreate_Approximator) == 0) return reinterpret_cast<void(*)>(internal::create_approximator_not_impl);
			if (strcmp(symbol_name, rsSolve_Model_Parameters) == 0) return reinterpret_cast<void(*)>(internal::solve_model_parameters_not_impl);
#endif

			return reinterpret_cast<void(*)>(internal::get_desc_not_impl);
		}

	}

}