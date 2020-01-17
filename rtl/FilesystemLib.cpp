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
 * Univerzitni 8
 * 301 00, Pilsen
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

#ifdef __APPLE__
	// small hack to avoid redefinition of TRUE and FALSE in non-strongly typed manner
	// this "converts" Apple definition of DYLD_BOOL to strongly typed enum, therefore
	// allowing us to define our own "global" TRUE and FALSE
	#define DYLD_BOOL class DYLD_BOOL

	#include <mach-o/dyld.h>
#endif


#include "FilesystemLib.h"

#include "hresult.h"
#include "../utils/winapi_mapping.h"
#include "../utils/string_utils.h"

#include <cstring>

std::wstring Get_Application_Dir() {

	const size_t bufsize = 1024;

#ifdef _WIN32
	wchar_t ModuleFileName[bufsize];
	GetModuleFileNameW(((HINSTANCE)&__ImageBase), ModuleFileName, bufsize);
#elif __APPLE__
	char RelModuleFileName[bufsize];
	uint32_t size = static_cast<uint32_t>(bufsize);
	_NSGetExecutablePath(RelModuleFileName, &size);

	char ModuleFileName[bufsize];
	realpath(RelModuleFileName, ModuleFileName);
#else
	char ModuleFileName[bufsize];
	memset(ModuleFileName, 0, bufsize);
	readlink("/proc/self/exe", ModuleFileName, bufsize);
	// TODO: error checking
#endif


#ifdef DHAS_FILESYSTEM
	filesystem::path exePath{ ModuleFileName };

	return exePath.remove_filename().wstring();
#else
	std::string spath(ModuleFileName);
	std::wstring path(spath.begin(), spath.end());

	size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
		path = path.substr(0, pos + 1);

	return path;
#endif
}

std::wstring& Path_Append(std::wstring& path, const wchar_t* level) {
#ifdef DHAS_FILESYSTEM
	// use overloaded operator/, which uses preferred path component separator
	path = (filesystem::path(path) / level).wstring();
#else
	if (!path.empty()) {
		if (path.substr(path.size() - 1, 1) != L"/")
			path += L"/";
	}
	path += level;
#endif
	return path;
}

bool Is_Directory(const std::wstring& path)
{
#ifdef DHAS_FILESYSTEM
	return filesystem::is_directory(path);
#elif defined(_WIN32)
	DWORD dwAttrib = GetFileAttributes(Narrow_WString(path).c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
	struct stat statbuf;
	if (stat(Narrow_WString(path).c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
#endif
}

bool Is_Regular_File_Or_Symlink(const std::wstring& path)
{
#ifdef DHAS_FILESYSTEM
	return filesystem::is_regular_file(path) || filesystem::is_symlink(path);
#elif defined(_WIN32)
	return !Is_Directory(path); // good enough
#else
	// no need to check for symlink, stat follows symbolic links
	struct stat statbuf;
	if (stat(Narrow_WString(path).c_str(), &statbuf) != 0)
		return false;
	return S_ISREG(statbuf.st_mode);
#endif
}
