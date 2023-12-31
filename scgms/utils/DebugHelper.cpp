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

#include "DebugHelper.h"

#if defined(_MSC_VER) && defined(_DEBUG)
    #include <wchar.h>
	#include <stdlib.h>
	#include <Windows.h>

	extern "C" char __ImageBase;
#endif

class CMemory_Leak_Tracker {
	//just a class-helper that correctly catches memory allocated with global const variable
protected:
#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtMemState mStart = { 0 }, mStop = { 0 }, mDiff = { 0 };
#endif
public:
	CMemory_Leak_Tracker();
	~CMemory_Leak_Tracker();
};

const CMemory_Leak_Tracker Memory_Leak_Tracker;

CMemory_Leak_Tracker::CMemory_Leak_Tracker() {
	#ifdef TrackLeaks
		#if defined(_MSC_VER) && defined(_DEBUG)
			#ifdef prefer_vld
				VLDEnable();
			#else
				_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_DELAY_FREE_MEM_DF);
				_CrtMemCheckpoint(&mStart); //take the initial snapshot
			#endif
		#endif
	#endif
}

CMemory_Leak_Tracker::~CMemory_Leak_Tracker() {
	#ifdef TrackLeaks
		#if defined(_MSC_VER) && defined(_DEBUG)
	
#ifndef _KERNEL_MODE
		try {
#endif
				_CrtMemCheckpoint(&mStop); //take the stop snapshot

				//wchar_t *fileName = (wchar_t*) malloc(1024*sizeof(wchar_t));
				//wchar_t *buf = (wchar_t*) malloc(2048*sizeof(wchar_t));
				//can't be on the heap, otherwise they will be reported as leaks
				static wchar_t fileName[1024];
				static wchar_t buf[2048];
				GetModuleFileNameW(((HINSTANCE)&__ImageBase), fileName, 1024);

				swprintf_s(buf, 2048, L"========== Dumping leaks for: %s ==========\n", fileName);
				OutputDebugStringW(buf);

				#ifdef prefer_vld
					VLDReportLeaks();
				#else				
				if (_CrtMemDifference(&mDiff, &mStart, &mStop) == TRUE) {
					OutputDebugStringW(L"-----------_CrtMemDumpStatistics ---------\n");
					_CrtMemDumpStatistics(&mDiff);
					OutputDebugStringW(L"-----------_CrtMemDumpAllObjectsSince ---------\n");
					_CrtMemDumpAllObjectsSince(&mStart);
					OutputDebugStringW(L"-----------_CrtDumpMemoryLeaks ---------\n");
					_CrtDumpMemoryLeaks();
				}
				else {
					OutputDebugStringW(L"No leaks detected:)\n");
				}							
				#endif

				swprintf_s(buf, 2048, L"========== Leaks dumped for: %s ==========\n", fileName);
				OutputDebugStringW(buf);
#ifndef _KERNEL_MODE
		}
		catch (...) {
			OutputDebugStringW(L"Something went wrong while trying to print the leaks!");
		}
#endif


		#endif
	#endif
}

void dprintf(std::stringstream &stream) {
        const auto msg = stream.str();
        OutputDebugStringA(msg.c_str());
}

void dprintf(std::string &str) {
	OutputDebugStringA(str.c_str());
}

void dprintf(const char *str) {
	OutputDebugStringA(str);
}
