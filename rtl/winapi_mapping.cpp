#include "winapi_mapping.h"

#ifdef _WIN32

#else

#include <string>

void* LoadLibraryW(const wchar_t *libname) {
	std::string libstr{libname, libname + wcslen(libname)};
	return (dlopen(libstr.c_str(), RTLD_LAZY));
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

#endif
