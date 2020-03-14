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

#pragma once

#include <stdint.h>
#include "..\..\common\rtl\guid.h"
#include "..\..\common\utils\winapi_mapping.h"

#define SimpleCalling __cdecl

typedef struct _TSCGMS_Event_Data {
	uint8_t event_code;
	GUID device_id;				//supporting parallel measurements
	GUID signal_id;				//blood, ist, isig, model id aka e.g, calculated blood, etc.

	double device_time;				//signal with multiple values are aggregated by device_time with the same signal_id and device_id
	int64_t logical_time;

	uint64_t segment_id;			// segment identifier or Invalid_Segment_Id

	//the following members are not in a union to make it easier for certain languages
	double level;					//level event

	double *parameters;				//parameters event
	size_t count;

	wchar_t *str;					//info event
} TSCGMS_Event_Data;

typedef void* scgms_execution_t;

typedef HRESULT(SimpleCalling *TSCGMS_Execution_Callback)(TSCGMS_Event_Data *event);
typedef scgms_execution_t(SimpleCalling *TExecute_SCMGS_Configuration)(const char *config, TSCGMS_Execution_Callback callback);
	
typedef BOOL(SimpleCalling *TInject_SCGMS_Event)(const scgms_execution_t execution, const TSCGMS_Event_Data *event);

typedef void(SimpleCalling *TShutdown_SCGMS)(const scgms_execution_t execution, BOOL wait_for_shutdown);