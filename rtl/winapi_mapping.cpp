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
	void* mem;
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
