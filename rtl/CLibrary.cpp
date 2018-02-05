#include "CLibrary.h"

#ifdef _WIN32
const wchar_t* rsShared_Object_Extension = L".dll";
#else
const wchar_t* rsShared_Object_Extension = L".so";
#endif

CLibrary::CLibrary() : mHandle(nullptr)
{
	//
}

CLibrary::~CLibrary()
{
	if (mHandle)
		Unload();
}

void CLibrary::Set_Filename(const std::wstring& path)
{
	mModulePath = path;
}

bool CLibrary::Load() {
	mHandle = LoadLibraryW(mModulePath.c_str());

	return mHandle != 0;
}

bool CLibrary::Is_Loaded() const
{
	return (mHandle != nullptr);
}

void CLibrary::Unload()
{
	if (mHandle)
	{
		FreeLibrary(mHandle);
		mHandle = nullptr;
	}
}

void* CLibrary::Resolve(const char* symbolName)
{
	if (!mHandle)
		return nullptr;

	return GetProcAddress(mHandle, symbolName);
}

bool CLibrary::Is_Library(const std::wstring& path)
{
	size_t extLen = wcslen(rsShared_Object_Extension);

	return (path.length() > extLen) && path.substr(path.length() - extLen, extLen) == rsShared_Object_Extension;
}
