#pragma once

#ifdef _WIN32
	#include <Windows.h>

	extern "C" char __ImageBase;

	using socklen_t = int;
#else
	#include <unistd.h>
	#include <dlfcn.h>

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
#endif
