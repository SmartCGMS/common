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

#ifdef _WIN32
	#include <Windows.h>

	extern "C" char __ImageBase;

	using socklen_t = int;

	#define MainCalling __cdecl
#else
	#include <unistd.h>
	#include <dlfcn.h>
	#include <ctime>

	#define MainCalling

	using BOOL = int;
	constexpr int TRUE = 1;

	using HMODULE = void*;

	void localtime_s(struct tm* t, const time_t* tim);
	void gmtime_s(struct tm* t, const time_t* tim);
	void _get_timezone(long* tzdst);

	void* LoadLibraryW(const wchar_t *filename);
	void* GetProcAddress(void *libhandle, const char *symbolname);
	void FreeLibrary(void* libhandle);

	int closesocket(int fd);

	#define SOCKET int
	#define InetPtonA inet_pton
	#define InetNtopA inet_ntop
	#define WSAGetLastError() errno

	#define swscanf_s swscanf

	void* _aligned_malloc(size_t n, size_t alignment);
	void _aligned_free(void* _Block);

	int wcstombs_s(size_t* converted, char* dst, size_t dstSizeBytes, const wchar_t* src, size_t maxSizeBytes);
#endif
