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

#include <algorithm>
#include <cwchar>

filesystem::path CDynamic_Library::mLibrary_Base{};

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
    mLibrary_Base = std::move(other.mLibrary_Base);
}

CDynamic_Library::~CDynamic_Library() {
	if (mHandle)
		Unload();
}

bool CDynamic_Library::Load(const filesystem::path &file_path) {
    if (mLibrary_Base.empty()) mLib_Path = file_path;
		else mLib_Path = mLibrary_Base / file_path;
         

    const auto converted_path = mLib_Path.wstring();
    mHandle = LoadLibraryW(converted_path.c_str());

	return mHandle != NULL;
}

filesystem::path CDynamic_Library::Lib_Path() const {
    return mLib_Path;
}

bool CDynamic_Library::Is_Loaded() const {
	return (mHandle != nullptr);
}

void CDynamic_Library::Unload() {
	if (mHandle) {
		FreeLibrary(mHandle);
		mHandle = NULL;
	}
}

void* CDynamic_Library::Resolve(const char* symbolName) {
	if (!mHandle)
		return nullptr;

	return GetProcAddress(mHandle, symbolName);
}

bool CDynamic_Library::Is_Library(const filesystem::path& path) {
	const auto ext = path.extension();

	if (ext.empty()) return false;
	return ext.wstring() == rsShared_Object_Extension;
}

void CDynamic_Library::Set_Library_Base(const filesystem::path& base) {
	mLibrary_Base = base;
}

filesystem::path CDynamic_Library::Get_Library_Base() {	
	return mLibrary_Base;
}

filesystem::path CDynamic_Library::Default_Extension() {
	return rsShared_Object_Extension;
}