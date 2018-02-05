#include "FilesystemLib.h"

#include <filesystem>

namespace filesystem = std::experimental::filesystem;

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

std::list<std::wstring> List_Directory(std::wstring basePath) {

	std::list<std::wstring> result;

	filesystem::path base(basePath);
	if (!base.empty()) {	

		// just to be sure all paths are using preferred separators
		base.make_preferred();

		for (auto& item : filesystem::directory_iterator(base))
			result.push_back(item.path().wstring());
	}

	return result;
}
