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
