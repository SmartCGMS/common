#include "Dynamic_Library.h"

#ifdef _WIN32
const wchar_t* rsShared_Object_Extension = L".dll";
#else
const wchar_t* rsShared_Object_Extension = L".so";
#endif

CDynamic_Library::CDynamic_Library() : mHandle(nullptr) {	
}

CDynamic_Library::~CDynamic_Library() {
	if (mHandle)
		Unload();
}

void CDynamic_Library::Set_Filename(const std::wstring& path) {
	mModulePath = path;
}

bool CDynamic_Library::Load() {
	mHandle = LoadLibraryW(mModulePath.c_str());

	return mHandle != 0;
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