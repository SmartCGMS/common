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


const size_t Max_File_Path = 1024;


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
	const bool result = filesystem::is_directory(path, ec);
	return result && !ec;
}

bool Is_Regular_File_Or_Symlink(const filesystem::path& path) {
	std::error_code ec;

	bool is_reg = filesystem::is_regular_file(path, ec);
	is_reg &= !ec;	
	if (is_reg) return true;

	bool is_sym = filesystem::is_symlink(path, ec);
	return is_sym & !ec;
}