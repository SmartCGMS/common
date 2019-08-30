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

#include "Dynamic_Library.h"

#include <algorithm>

std::wstring CDynamic_Library::mLibrary_Base{ L"" };

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
}

CDynamic_Library::~CDynamic_Library() {
	if (mHandle)
		Unload();
}


bool CDynamic_Library::Load(const wchar_t *file_path) {
	mHandle = LoadLibraryW((mLibrary_Base + file_path).c_str());

	return mHandle != NULL;
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

bool CDynamic_Library::Is_Library(const std::wstring& path) {
	size_t extLen = wcslen(rsShared_Object_Extension);

	return (path.length() > extLen) && path.substr(path.length() - extLen, extLen) == rsShared_Object_Extension;
}

void CDynamic_Library::Set_Library_Base(const std::wstring& base)
{
	mLibrary_Base = base;
	if (mLibrary_Base.empty() || (*mLibrary_Base.rbegin() != L'/' && *mLibrary_Base.rbegin() != L'\\'))
		mLibrary_Base += L"/";
}

const wchar_t* CDynamic_Library::Get_Library_Base()
{
	return mLibrary_Base.c_str();
}
