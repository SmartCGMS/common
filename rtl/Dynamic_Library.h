#pragma once

#include <string>

#include "referencedImpl.h"
#include "winapi_mapping.h"

/*
 * Dynamic library (shared object) wrapper class
 */
class CDynamic_Library final
{
	private:
		// stored module handle (nullptr if invalid)
		HMODULE mHandle;
		// original module path
		std::wstring mModulePath;

	public:
		CDynamic_Library();
		// disallow copying - the handle has to be unique
		CDynamic_Library(const CDynamic_Library&) = delete;
		virtual ~CDynamic_Library();

		// sets library module filename
		void Set_Filename(const std::wstring& path);
		// loads module and returns result
		bool Load();
		// unloads module if loaded
		void Unload();
		// resolves symbol from loaded module; returns nullptr if no such symbol found or no module loaded
		void* Resolve(const char* symbolName);

		// is module (properly) loaded?
		bool Is_Loaded() const;

		// checks extension of supplied path to verify, if it's a library (platform-dependent check)
		static bool Is_Library(const std::wstring& path);
};