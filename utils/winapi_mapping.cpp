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
 *       monitoring", Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 */

#include "winapi_mapping.h"

#ifdef _WIN32

#else

#include <string>

void* LoadLibraryW(const wchar_t *libname) {
	std::string libstr{libname, libname + wcslen(libname)};
	return (dlopen(libstr.c_str(), RTLD_LOCAL|RTLD_NOW));
}

void *GetProcAddress(void *libhandle, const char *symbolname) {
	return dlsym(libhandle, symbolname);
}

void FreeLibrary(void* libhandle) {
	dlclose(libhandle);
}
	
int closesocket(int fd) {
	return close(fd);
}

#include <ctime>

void localtime_s(struct tm* t, const time_t* tim)
{
	localtime_r(tim, t);
}

void gmtime_s(struct tm* t, const time_t* tim)
{
	gmtime_r(tim, t);
}

void* _aligned_malloc(size_t n, size_t alignment)
{
	void* mem = nullptr;
	posix_memalign(&mem, alignment, n);

	return mem;
}

void _aligned_free(void* _Block)
{
	free(_Block);
}

int wcstombs_s(size_t* converted, char* dst, size_t dstSizeBytes, const wchar_t* src, size_t maxSizeBytes)
{
	return wcstombs(dst, src, maxSizeBytes);
}

#endif
