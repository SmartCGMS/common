/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

// feature check for C++17/TS support state - filesystem is supported on both MSVS2017 and GCC8, but
// on MSVS it's still in experimental namespace, contrary to GCC8, where it's considered stable
// we may also decide to disable C++17 FS support on some platforms (e.g.; some Android targets) with STDCPP_FS_DISABLED macro
#if !defined(STDCPP_FS_DISABLED) && __has_include(<filesystem>)
	#include <filesystem>
	namespace filesystem = std::filesystem;
	#define DHAS_FILESYSTEM
#elif !defined(STDCPP_FS_DISABLED) && __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace filesystem = std::experimental::filesystem;
	#define DHAS_FILESYSTEM
#else
	#define GHC_WIN_WSTRING_STRING_TYPE
	#include "../utils/filesystem.hpp"
	namespace filesystem = ghc::filesystem;
	#define DHAS_FILESYSTEM
#endif

#include <string>
#include <list>

#include "hresult.h"

// resolves application directory (may be different from working directory)
std::wstring Get_Application_Dir();

std::wstring Get_Dll_Dir();

// appends next level to supplied path using preferred separator
std::wstring& Path_Append(std::wstring& path, const wchar_t* level);

// does given path point to a directory?
bool Is_Directory(const std::wstring& path);

// does given path point to a directory or is a symbolic link?
bool Is_Regular_File_Or_Symlink(const std::wstring& path);

// lists directory at supplied path; fills target list with absolute paths
template <typename A = std::allocator<std::wstring>>
std::list<std::wstring, A> List_Directory(std::wstring basePath) {

	std::list<std::wstring, A> result;

#ifdef DHAS_FILESYSTEM
	filesystem::path base(basePath);
	if (!base.empty()) {

		// just to be sure all paths are using preferred separators
		base.make_preferred();

		for (auto& item : filesystem::directory_iterator(base))
			result.push_back(item.path().wstring());
	}
#else
	std::string sbase(basePath.begin(), basePath.end());

	if (*basePath.rbegin() != L'/')
		basePath += L"/";

	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(sbase.c_str())) != NULL) {

		while ((dirp = readdir(dp)) != NULL)
			result.push_back(basePath + std::wstring(dirp->d_name, dirp->d_name + strlen(dirp->d_name)));

		closedir(dp);
	}
#endif

	return result;
}

std::wstring Native_Slash(const wchar_t* path);