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

#define TrackLeaks	
	//Sometimes, e.g. SaveFileDialog with Qt causes hangup
	//not that new(std::nothrow) will not compile with CrtDbg-like new_operator_replacement

#ifdef TrackLeaks

	//Visual Leak Detector - vld.codeplex.com
	//This will cause problems, if a dll enabling VLD is loaded in a thread that does not finish as the last one.
	// VLD should never be enabled again as the VS2019 is unofficial, and VS2022 is none.
//	#define prefer_vld

	#if defined(_MSC_VER) 
//&& defined(_DEBUG)
		#ifdef prefer_vld
			// local-specific path to VLD; left here for compatibility/legacy reasons
			#if __has_include("../../../Third Party/Visual Leak Detector/include/vld.h")
				#include "../../../Third Party/Visual Leak Detector/include/vld.h"
			#else
				#include <vld.h>
			#endif
		#else

			//The Thread Building Blocks has to include prior redefining new
			#ifdef TBB_USE_DEBUG
				#include "tbb/tbb.h"
				#include "tbb/task.h"
				#include "tbb/atomic.h"
			#endif

			#define _CRTDBG_MAP_ALLOC
			#include <crtdbg.h>

/*			The following defines are obsolete constructions, which cause problems
*			when using new(std::nothow) type_t[size_t];
* 
			#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
			#define DEBUG_NORMALBLOCK   new( _NORMAL_BLOCK, __FILE__, __LINE__)
			#define new_operator_replacement DEBUG_CLIENTBLOCK
			#define new new_operator_replacement
*/
		#endif
	#endif

#endif

#include <iostream>
#include <sstream>
#include <fstream>

#define overridedefdprintf

#if defined(_MSC_VER) && !defined(overridedefdprintf)
	#include <Dbgeng.h>	//provides dprintf
#else

#ifdef _WIN32
	#include <Windows.h>
#else
	#define OutputDebugStringA(x) (void)(x) // to avoid "unused variable" warning
	#define OutputDebugStringW(x) (void)(x)
	#define swprintf_s swprintf
	#define sprintf_s(x, y, z, a)
#endif

	template <typename... Args>
	void dprintf(const char *format, Args... args) {
		const size_t bufsize=2048;
		char buf[bufsize];
		sprintf_s(buf, bufsize, format, args...);
		OutputDebugStringA(buf); 
	}

	template <typename... Args>
	void dprintf(const wchar_t *format, Args... args) {
		const size_t bufsize=2048;
		wchar_t buf[bufsize];
		swprintf_s(buf, bufsize, format, args...);
		OutputDebugStringW(buf); 
	}

	template <typename T>
	void dprintf(T arg) {
		std::stringstream stream;
		stream << arg;
		const auto msg = stream.str();
		OutputDebugStringA(msg.c_str());
	}

	void dprintf(std::stringstream &stream);
	void dprintf(std::string &str);
	void dprintf(const char *str);

#endif
