#pragma once

#include <string>
#include <list>

#include "hresult.h"

// resolves application directory (may be different from working directory)
std::wstring Get_Application_Dir();

// appends next level to supplied path using preferred separator
std::wstring& Path_Append(std::wstring& path, const wchar_t* level);

// lists directory at supplied path; fills target list with absolute paths
std::list<std::wstring> List_Directory(std::wstring basePath);
