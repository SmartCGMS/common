#include "FilesystemLib.h"


#include "hresult.h"
#include "winapi_mapping.h"

#include <cstring>

std::wstring Get_Application_Dir()
{

	const size_t bufsize = 1024;

#ifdef _WIN32
	wchar_t ModuleFileName[bufsize];
	GetModuleFileNameW(((HINSTANCE)&__ImageBase), ModuleFileName, bufsize);
#else
	char ModuleFileName[bufsize];
	memset(ModuleFileName, 0, bufsize);
	readlink("/proc/self/exe", ModuleFileName, bufsize);
	// TODO: error checking
#endif

	filesystem::path exePath { ModuleFileName};

	return exePath.remove_filename().wstring();
}

std::wstring& Path_Append(std::wstring& path, const wchar_t* level)
{
	// use overloaded operator/, which uses preferred path component separator
	path = (filesystem::path(path) / level).wstring();
	return path;
}

