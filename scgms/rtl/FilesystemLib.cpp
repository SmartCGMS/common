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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#ifdef __APPLE__
	// small hack to avoid redefinition of TRUE and FALSE in non-strongly typed manner
	// this "converts" Apple definition of DYLD_BOOL to strongly typed enum, therefore
	// allowing us to define our own "global" TRUE and FALSE
	#define DYLD_BOOL class DYLD_BOOL

	#include <mach-o/dyld.h>
	#include <dlfcn.h>
#endif


#include "FilesystemLib.h"

#include "hresult.h"
#include "../utils/winapi_mapping.h"

#include <cstring>
#include <algorithm>
#include <cwctype>

// PATH_MAX is fixed on several platforms (e.g. Android) to a fairly big number (4096). We don't need so long paths on a regular system,
// so we define it to be lower. However, Android-NDK verifies the length of an input buffer to be >= PATH_MAX, so we have to be consistent
#ifdef PATH_MAX
	constexpr size_t Max_File_Path = PATH_MAX;
#else
	//On Windows, there's MAX_PATH==260, which is too little => hence we do not check for its presence
	constexpr size_t Max_File_Path = 1024;
#endif


filesystem::path Get_Application_Dir() {

#ifdef _WIN32
	wchar_t ModuleFileName[Max_File_Path];
	GetModuleFileNameW(NULL, ModuleFileName, Max_File_Path);
#elif __APPLE__
	char RelModuleFileName[Max_File_Path];
	uint32_t size = static_cast<uint32_t>(Max_File_Path);
	_NSGetExecutablePath(RelModuleFileName, &size);

	char ModuleFileName[Max_File_Path];
	realpath(RelModuleFileName, ModuleFileName);
#else
	char ModuleFileName[Max_File_Path];
	memset(ModuleFileName, 0, Max_File_Path);
	readlink("/proc/self/exe", ModuleFileName, Max_File_Path);
	// TODO: error checking
#endif

	filesystem::path exe_path{ ModuleFileName };

	return exe_path.remove_filename();
}

filesystem::path Get_Dll_Dir() {
	
#ifdef _WIN32
	wchar_t ModuleFileName[Max_File_Path];
	GetModuleFileNameW(((HINSTANCE)&__ImageBase), ModuleFileName, Max_File_Path);
#else
	char ModuleFileName[Max_File_Path];
	Dl_info info;
	if (dladdr((void*)Get_Dll_Dir, &info) != 0) {
		auto result = realpath(info.dli_fname, ModuleFileName);
		if (!result)
			return Get_Application_Dir();
	}
	else
		return Get_Application_Dir();
#endif

	filesystem::path dll_path{ ModuleFileName };

	return dll_path.remove_filename();
}


bool Is_Directory(const filesystem::path& path) {
	std::error_code ec;

	const auto path_status = filesystem::status(path, ec);
	bool result = !ec;
	if (result)
		result = filesystem::is_directory(path_status);

	return result;
}

bool Is_Regular_File_Or_Symlink(const filesystem::path& path) {
	std::error_code ec;

	bool is_reg = filesystem::is_regular_file(path, ec);
	is_reg &= !ec;
	if (is_reg) return true;

	bool is_sym = filesystem::is_symlink(path, ec);
	return is_sym && (!ec);
}

std::wstring& Ensure_Uniform_Dir_Separator(std::wstring& path) noexcept {
	bool forward_slash = false;
	bool back_slash = false;

	//check if the string has uniform separator
	for (const auto& e : path) {
		switch (e) {
			case L'\\' : back_slash = true;
						  break;

			case L'/': forward_slash = true;
						break;

			default:
				break;
		}
	}

	//if there's a mismatch, attempt to correct it
	if (forward_slash && back_slash) {
#ifdef _WIN32
		const wchar_t sep = L'\\';
#else
		const wchar_t sep = L'/';
#endif

		for (auto& e : path) {
			switch (e) {
				case L'\\':
				case L'/': e = sep;
					break;
			default:
				break;
			}
		}
	}

	return path;
}


bool Match_Wildcard(const std::wstring fname, const std::wstring wcard, const bool case_sensitive) {
	size_t f = 0;
	for (size_t w = 0; w < wcard.size(); w++) {
		switch (wcard[w]) {
		case L'?':
			if (f >= fname.size()) return false;
			//there is one char to eat, let's continue
			f++;
			break;


		case L'*':
			//skip everything in the filename until extension or dir separator
			while (f < fname.size()) {
				if ((fname[f] == L'.') || (fname[f] == filesystem::path::preferred_separator)) 
					break;

				f++;
			}
			break;

		default:
			if (f >= fname.size())
				return false;
			if (case_sensitive) {
				if (wcard[w] != fname[f])
					return false;
			}
			else {
				if (std::towupper(wcard[w]) != std::towupper(fname[f]))
					return false;
			}
			//wild card and name still matches, continue
			f++;
			break;
		}
	}

	return f < fname.size() ? false : true;	//return false if some chars in the fname were not eaten
}

std::wstring Make_Absolute_Path(filesystem::path src_path, filesystem::path parent_path) {
	if (src_path.is_relative()) {
		std::error_code ec;
		filesystem::path relative_part = src_path;
		src_path = filesystem::canonical(parent_path / relative_part, ec);
		if (ec) {
			src_path = filesystem::weakly_canonical(parent_path / relative_part, ec);
			if (ec)
				src_path = parent_path / relative_part;
		}
	}

	src_path = src_path.make_preferred();	//we know that make_preferred fails sometimes
	std::wstring converted_path = src_path.wstring();
	converted_path = Ensure_Uniform_Dir_Separator(converted_path);

	return converted_path;
}