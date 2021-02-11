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

#pragma once

#include <string>
#include <memory>

#include "referencedImpl.h"
#include "../rtl/FilesystemLib.h"
#include "../utils/winapi_mapping.h"

/*
 * Dynamic library (shared object) wrapper class
 */
class CDynamic_Library final {
	private:
		// stored module handle (nullptr if invalid)
		HMODULE mHandle;
		// library base for given platform (sometimes needs to be set in runtime, on e.g. Android)
		static filesystem::path mLibrary_Base;	// to avoid a memory leak due to TBB allocator
		filesystem::path mLib_Path;
	public:
		CDynamic_Library() noexcept;
		// disallow copying - the handle has to be unique
		CDynamic_Library(const CDynamic_Library&) = delete;
		CDynamic_Library(CDynamic_Library&& other) noexcept;
		virtual ~CDynamic_Library() noexcept;

		// loads module and returns result
		bool Load(const filesystem::path &file_path) noexcept;
		// unloads module if loaded
		void Unload() noexcept;
		// resolves symbol from loaded module; returns nullptr if no such symbol found or no module loaded
		void* Resolve(const char* symbolName) noexcept;

		template<typename T>
		T Resolve(const char* symbolName) noexcept {
			return reinterpret_cast<T>(Resolve(symbolName));
		}

		// is module (properly) loaded?
		bool Is_Loaded() const noexcept;
        filesystem::path Lib_Path() const noexcept;

		// checks extension of supplied path to verify, if it's a library (platform-dependent check)
		static bool Is_Library(const filesystem::path& path) noexcept;
		// sets library base
		static void Set_Library_Base(const filesystem::path& base) noexcept;
		// retrieves library base directory
		static filesystem::path Get_Library_Base() noexcept;

		static filesystem::path Default_Extension() noexcept;
};
