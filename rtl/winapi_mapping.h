#pragma once

#ifdef _WIN32
	#include <Windows.h>

	extern "C" char __ImageBase;

	using socklen_t = int;
#else
	#include <unistd.h>
	#include <dlfcn.h>

	using HMODULE = void*;

	void localtime_s(struct tm* t, const time_t* tim);
	void gmtime_s(struct tm* t, const time_t* tim);
	void _get_timezone(long* tzdst);

	void* LoadLibraryW(const wchar_t *filename);
	void* GetProcAddress(void *restrict libhandle, const char *restrict symbolname);
	void FreeLibrary(void* libhandle);

	#define LoadLibraryA(libname) dlopen(libname, RTLD_LAZY)
	#define GetProcAddress(libhandle, symbolname) dlsym(libhandle, symbolname)
	#define FreeLibrary(libhandle) dlclose(libhandle)

	#define SOCKET int
	#define closesocket close
	#define InetPtonA inet_pton
	#define InetNtopA inet_ntop
	#define WSAGetLastError() errno
#endif
