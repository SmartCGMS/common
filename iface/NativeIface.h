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
}

	//In the native script, let's forbidd almost all values, except the state, from a modification,
	//while allowing the modification in the SCGMS filter.
#ifdef SCGMS_SCRIPT
	#define DCONST const
#else
	#define DCONST 
#endif



#if defined(SCGMS_SCRIPT) && defined(DCustom_Data_Name)
	class DCustom_Data_Name;
#endif	

struct TNative_Environment {
	DCONST native::TSend_Event send;					//function to inject new events
	
#if defined(SCGMS_SCRIPT) && defined(DCustom_Data)
	DCustom_Data * const custom_data;					//custom data pointer to implement a stateful processing
#else
	const void* custom_data;							//custom data pointer to implement a stateful processing
#endif

	DCONST size_t current_signal_index;
	DCONST size_t level_count;							//number of levels to sanitize memory space - should be generated
	DCONST GUID signal_id[native::max_signal_count];	//signal ids as configured
	DCONST double device_time[native::max_signal_count];//recent device times
	DCONST double level[native::max_signal_count];		//recent levels
	DCONST double slope[native::max_signal_count]; 		//recent slopes from the recent level to the preceding level, a linear line slope!
	
	DCONST double parameters[native::max_parameter_count];//configurable parameters
};


#if defined(SCGMS_SCRIPT) && defined(DCustom_Data_Name) && defined(DCustom_Data_Def)
	DCustom_Data_Def
#endif

using TNative_Execute_Wrapper = HRESULT(IfaceCalling*)(
		const std::underlying_type_t<scgms::NDevice_Event_Code> reason,
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
#endif

#ifdef SCGMS_SCRIPT
	
	//Let's declare the raw-execute function, from which we will call the syntactic-suger execute
	void execute(GUID &sig_id, double &device_time, double &level,
		HRESULT &rc, TNative_Environment &environment, const void* context);

	//DLL_EXPORT so that this function needs no .cpp file and hence does not get ignored by the compiler
	DLL_EXPORT HRESULT IfaceCalling execute_wrapper(
		const std::underlying_type_t<scgms::NDevice_Event_Code> reason,
		GUID* sig_id, double* device_time, double* level,
		TNative_Environment* environment, const void* context) {
			

		HRESULT rc = S_OK;

		auto Handle_Segment_Start = [&]() {
	#ifdef DCustom_Data

			rc = E_FAIL;	//be ready for a catastrophe
			std::unique_ptr<DCustom_Data> state = std::make_unique<DCustom_Data>();
			if (state) {
				if (state->Init(*device_time, *environment, context)) {
		/*			//Init succeeded => move the pointer
					using q = decltype(environment->custom_data);
					using w = std::remove_cv<q>;
					auto z = reinterpret_cast<w>(environment->custom_data);
					reinterpret_cast<DCustom_Data*>(environment->custom_data) = state.get();
					state.release();
			*/		rc = S_OK;
				}
			
			}
			/*
			DCustom_Data** modifiable = &(reinterpret_cast<DCustom_Data*>(environment->custom_data));
			*modifiable = new DCustom_Data();

			constexpr size_t sz = custom_data_sizeof<TCustom_Data>;
			if constexpr (sz != 0) {
				
				
				using TEffective_Custom_Data = std::decay<TCustom_Data_Ptr>;
				TEffective_Custom_Data* ptr = new TEffective_Custom_Data();				
				
				modifiable->custom_data = reinterpret_cast<TCustom_Data_Ptr>(ptr);

				if (ptr != nullptr) {
					if constexpr (has_init_state<TCustom_Data>::value) {
						Init_State(modifiable->custom_data, );
						//ptr->
					}					
				} else
					rc = E_FAIL;
			}
			*/
	#endif
		};

		auto Handle_Segment_Stop = [&]() {
			/*if constexpr (custom_data_sizeof<TCustom_Data> != 0) {
				if (environment->custom_data) {
					using TEffective_Custom_Data = std::decay_t<TCustom_Data_Ptr>;
					TNative_Environment* modifiable = const_cast<TNative_Environment*>(environment);
					delete reinterpret_cast<TEffective_Custom_Data>(modifiable->custom_data);
					modifiable->custom_data = nullptr;
				}			
			}
			*/
		};

		
		
		switch (static_cast<scgms::NDevice_Event_Code>(reason)) {
		
			case scgms::NDevice_Event_Code::Time_Segment_Start: Handle_Segment_Start();
				break;

			case scgms::NDevice_Event_Code::Time_Segment_Stop: Handle_Segment_Stop();
				break;


			case scgms::NDevice_Event_Code::Level:
				execute(*sig_id, *device_time, *level, rc, *environment, context);
				break;

			default: break;
		}

		return rc;
	}
#endif