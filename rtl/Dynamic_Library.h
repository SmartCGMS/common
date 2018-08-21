#pragma once

#include <string>

#include "referencedImpl.h"
#include "winapi_mapping.h"

/*
 * Dynamic library (shared object) wrapper class
 */
class CDynamic_Library final {
	private:
		// stored module handle (nullptr if invalid)
		HMODULE mHandle;
	public:
		CDynamic_Library() noexcept;
		// disallow copying - the handle has to be unique
		CDynamic_Library(const CDynamic_Library&) = delete;
		CDynamic_Library(CDynamic_Library&& other) noexcept;
		virtual ~CDynamic_Library();

		// loads module and returns result
		bool Load(const wchar_t *file_path);
		// unloads module if loaded
		void Unload();
		// resolves symbol from loaded module; returns nullptr if no such symbol found or no module loaded
		void* Resolve(const char* symbolName);

		// is module (properly) loaded?
		bool Is_Loaded() const;

		// checks extension of supplied path to verify, if it's a library (platform-dependent check)
		static bool Is_Library(const std::wstring& path);
};
