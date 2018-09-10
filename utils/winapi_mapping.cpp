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

#include "winapi_mapping.h"

#ifdef _WIN32

#else

#include <string>

void* LoadLibraryW(const wchar_t *libname) {
	std::string libstr{libname, libname + wcslen(libname)};
	return (dlopen(libstr.c_str(), RTLD_NOW));
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
