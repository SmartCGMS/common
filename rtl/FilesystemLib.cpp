#include "FilesystemLib.h"


#include "hresult.h"
#include "winapi_mapping.h"

std::wstring Get_Application_Dir()
{

	const size_t bufsize = 1024;
	wchar_t ModuleFileName[bufsize];

#ifdef _WIN32
	GetModuleFileNameW(((HINSTANCE)&__ImageBase), ModuleFileName, bufsize);
#else
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

