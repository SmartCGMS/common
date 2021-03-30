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

#pragma once

#include "DeviceIface.h"
#include "../rtl/hresult.h"


namespace native {
	constexpr size_t max_signal_count = 10;
	constexpr size_t max_parameter_count = 10; //number of configurable parameters

	using TSend_Event = HRESULT(IfaceCalling*)(const GUID* sig_id, const double device_time, const double level, const char* msg, const void* context);
	using TCustom_Data_Size = size_t(IfaceCalling*)();
}



#ifdef SCGMS_SCRIPT
	struct TCustom_Data;

	//let custom_data_sizeof be the sizeof(T)
	template<typename, typename = void>
	constexpr size_t custom_data_sizeof = 0;

	template<typename T>
	constexpr size_t custom_data_sizeof<T, std::void_t<decltype(sizeof(T))>> = sizeof(T);

	template<bool B, class T = void>
	struct Complete_Custom_Data { using TCustom_Data_Ptr = void*; };

	template<class T>
	struct Complete_Custom_Data<true, T> { using TCustom_Data_Ptr = TCustom_Data*; };

	using TCustom_Data_Ptr = Complete_Custom_Data<custom_data_sizeof<TCustom_Data> != 0, TCustom_Data>::TCustom_Data_Ptr;
	#define DNEC const
#else
	#define DNEC 
#endif

struct TNative_Environment {
	DNEC native::TSend_Event send;						//function to inject new events
#ifdef SCGMS_SCRIPT
	TCustom_Data_Ptr custom_data;						//custom data pointer to implement a stateful processing
#else
	DNEC void* custom_data;								//custom data pointer to implement a stateful processing
#endif

	DNEC size_t current_signal_index;
	DNEC size_t level_count;							//number of levels to sanitize memory space - should be generated
	DNEC GUID signal_id[native::max_signal_count];		//signal ids as configured
	DNEC double device_time[native::max_signal_count];  //recent device times
	DNEC double level[native::max_signal_count];		//recent levels
	DNEC double slope[native::max_signal_count]; 		//recent slopes from the recent level to the preceding level, a linear line slope!

	DNEC double parameters[native::max_parameter_count];//configurable parameters
};



using TNative_Execute_Wrapper = HRESULT(IfaceCalling*)(
		GUID* sig_id, double *device_time, double *level,
		TNative_Environment*environment, const void* context
	);


#if defined(_WIN32) && defined(SCGMS_SCRIPT)
	#define DLL_EXPORT	__declspec(dllexport)

	#include <Windows.h>

	BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
		return TRUE;
	}
#else
	#define DLL_EXPORT

	#ifndef APIENTRY
		#define APIENTRY
	#endif
#endif


#ifdef SCGMS_SCRIPT
	void execute(GUID &sig_id, double &device_time, double &level,
		HRESULT &rc, TNative_Environment &environment, const void* context);

	//DLL_EXPORT so that this function needs no .cpp file and hence does not get ignored by the compiler
	extern "C" DLL_EXPORT HRESULT IfaceCalling execute_wrapper(GUID* sig_id, double* device_time, double* level,
	TNative_Environment* environment, const void* context) {
			
		HRESULT rc = S_OK;
		execute(*sig_id, *device_time, *level, rc, *environment, context);
		return rc;
	}

	extern "C" size_t custom_data_size() {
		return custom_data_sizeof<TCustom_Data>;
	}

#endif