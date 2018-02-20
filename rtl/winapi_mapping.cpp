#include "winapi_mapping.h"

#ifdef _WIN32

#else

void* LoadLibraryW(const wchar_t *libname) {
	return (dlopen(libname, RTLD_LAZY));
}

void *GetProcAddress(void *restrict libhandle, const char *restrict symbolname) {
	return dlsym(libhandle, symbolname);
}

void FreeLibrary(void* libhandle) {
	return dlclose(libhandle);
}
	
int closesocket(int fd) {
	return close(fd);
}

#include <time.h>

void localtime_s(struct tm* t, const time_t* tim)
{
	localtime_r(&tim, t);
}

void gmtime_s(struct tm* t, const time_t* tim)
{
	gmtime_r(tim, t);
}

#endif
