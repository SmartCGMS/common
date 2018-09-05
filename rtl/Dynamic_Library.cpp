/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#include "Dynamic_Library.h"

#include <algorithm>

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
	mHandle = LoadLibraryW(file_path);

	return mHandle != nullptr;
}

bool CDynamic_Library::Is_Loaded() const {
	return (mHandle != nullptr);
}

void CDynamic_Library::Unload() {
	if (mHandle) {
		FreeLibrary(mHandle);
		mHandle = nullptr;
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
