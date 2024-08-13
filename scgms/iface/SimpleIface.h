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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#include <stdint.h>
#include <scgms/rtl/guid.h>
#include <scgms/rtl/hresult.h>
#include <scgms/utils/winapi_mapping.h>

#ifdef _WIN32
	#define SimpleCalling __cdecl
#else
	#define SimpleCalling
#endif

#pragma pack(push,1)

/* simplified device event structure for languages which may not support objects */
typedef struct _TSCGMS_Event_Data {
	/* event code this event contains */
	uint8_t event_code;

	/* GUID of an entity (filter, model, device, ...) that is a source of this event */
	GUID device_id;
	/* GUID of a signal contained within this entity; this may also be related to the information */
	GUID signal_id;

	/* device time encoded as rattime */
	double device_time;
	/* logical time of the device event (Lamport) */
	int64_t logical_time;

	/* segment identifier (for e.g., signal grouping); may be Invalid_Segment to indicate no segment */
	uint64_t segment_id;

	//the following members are not in a union to make it easier for certain languages
	/* signal level */
	double level;
	/* model parameters */
	double *parameters;
	/* parameters count; normally, we use a vector container, but since this simple interface is intended for
	 * languages which may not support objects, we marshal the parameters as such */
	size_t count;

	/* contained string - info, error, warning */
	wchar_t *str;
} TSCGMS_Event_Data;

#pragma pack(pop)

typedef void* scgms_execution_t;

typedef HRESULT(SimpleCalling *TSCGMS_Execution_Callback)(TSCGMS_Event_Data *event);
typedef scgms_execution_t(SimpleCalling *TExecute_SCMGS_Configuration)(const char *config, TSCGMS_Execution_Callback callback);
	
typedef BOOL(SimpleCalling *TInject_SCGMS_Event)(const scgms_execution_t execution, const TSCGMS_Event_Data *event);

typedef void(SimpleCalling *TShutdown_SCGMS)(const scgms_execution_t execution, BOOL wait_for_shutdown);