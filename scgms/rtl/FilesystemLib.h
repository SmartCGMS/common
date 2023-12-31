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

#pragma once

// feature check for C++17/TS support state - filesystem is supported on both MSVS2017 and GCC8, but
// on MSVC it's still in experimental namespace, contrary to GCC8, where it's considered stable
// we may also decide to disable C++17 FS support on some platforms (e.g.; some Android targets) with STDCPP_FS_DISABLED macro
#if !defined(STDCPP_FS_DISABLED) && __has_include(<filesystem>)
	#include <filesystem>
	namespace filesystem = std::filesystem;
#else
	#define GHC_WIN_WSTRING_STRING_TYPE
	#include "../utils/filesystem.hpp"
	namespace filesystem = ghc::filesystem;
#endif

#include <string>
#include <list>

#include "hresult.h"

// resolves application directory (may be different from working directory)
filesystem::path Get_Application_Dir();

filesystem::path Get_Dll_Dir();

// does given path point to a directory?
bool Is_Directory(const filesystem::path& path);

// does given path point to a directory or is a symbolic link?
bool Is_Regular_File_Or_Symlink(const filesystem::path& path);

std::wstring& Ensure_Uniform_Dir_Separator(std::wstring& path) noexcept;	//some std libs still fail to make_preferred

bool Match_Wildcard(const std::wstring fname, const std::wstring wcard, const bool case_sensitive);

std::wstring Make_Absolute_Path(filesystem::path src_path, filesystem::path parent_path); //returns wstring because src_path may be relative, or even include a wild-card