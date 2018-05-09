#pragma once

#include <filesystem>

namespace filesystem = std::experimental::filesystem;

#include <string>
#include <list>

#include "hresult.h"

// resolves application directory (may be different from working directory)
std::wstring Get_Application_Dir();

// appends next level to supplied path using preferred separator
std::wstring& Path_Append(std::wstring& path, const wchar_t* level);

// lists directory at supplied path; fills target list with absolute paths
template <typename A = std::allocator<std::wstring>>
std::list<std::wstring, A> List_Directory(std::wstring basePath) {

	std::list<std::wstring, A> result;

	filesystem::path base(basePath);
	if (!base.empty()) {

		// just to be sure all paths are using preferred separators
		base.make_preferred();

		for (auto& item : filesystem::directory_iterator(base))
			result.push_back(item.path().wstring());
	}

	return result;
}

