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

#include "Dynamic_Library.h"
#include "../utils/string_utils.h"

#include <algorithm>
#include <cwchar>

#ifdef _WIN32
	const wchar_t* rsShared_Object_Extension = L".dll";
#elif __APPLE__
	const wchar_t* rsShared_Object_Extension = L".dylib";
#else
	const wchar_t* rsShared_Object_Extension = L".so";
#endif

CDynamic_Library::CDynamic_Library() noexcept : mHandle(nullptr) {
}

CDynamic_Library::CDynamic_Library(CDynamic_Library&& other) noexcept : mHandle(nullptr) {
	std::swap(mHandle, other.mHandle);
	mLib_Path = std::move(other.mLib_Path);
}

CDynamic_Library::~CDynamic_Library() noexcept {
	if (mHandle)
		Unload();
}

bool CDynamic_Library::Load(const filesystem::path &file_path) noexcept {
	mLib_Path = file_path;

    const auto converted_path = mLib_Path.wstring();
    mHandle = LoadLibraryW(converted_path.c_str());

	// if the library was not found, and is requested by a relative path without leading dot, explicitly try to search in "current" directory;
	// GNU/Linux and macOS does not do so automatically, so when "libname.so" is requested, we need to explicitly try "./libname.so"
	// to find it in current location; the same applies to other relative paths, such as "filters/libname.so" --> "./filters/libname.so"
	if (mHandle == NULL && mLib_Path.is_relative() && !converted_path.empty() && converted_path[0] != '.') {
		// construct temporary path instance due to bug in assignment operator of filesystem lib (valgrind reports invalid reads and writes)
		const filesystem::path npath = filesystem::path{ std::wstring{ L"." } } / mLib_Path;
		mLib_Path = npath;
		const auto converted_path2 = mLib_Path.wstring();
		mHandle = LoadLibraryW(converted_path2.c_str());
	}

	return mHandle != NULL;
}

filesystem::path CDynamic_Library::Lib_Path() const noexcept {
    return mLib_Path;
}

bool CDynamic_Library::Is_Loaded() const noexcept {
	return (mHandle != nullptr);
}

void CDynamic_Library::Unload() noexcept {
	if (mHandle) {
		FreeLibrary(mHandle);
		mHandle = NULL;
		mLib_Path.clear();
	}
}

void* CDynamic_Library::Resolve(const char* symbolName) noexcept {
	if (!mHandle)
		return nullptr;

	return GetProcAddress(mHandle, symbolName);
}

bool CDynamic_Library::Is_Library(const filesystem::path& path) noexcept {
	const auto ext = path.extension();

	if (ext.empty()) return false;
	return ext.wstring() == rsShared_Object_Extension;
}

filesystem::path CDynamic_Library::Default_Extension() noexcept {
	return filesystem::path{ Narrow_WChar(rsShared_Object_Extension) };
}
