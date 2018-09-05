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

#pragma once

// feature check for C++17/TS support state - filesystem is supported on both MSVS2017 and GCC8, but
// on MSVS it's still in experimental namespace, contrary to GCC8, where it's considered stable
#if __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#elif __has_include(<filesystem>)
#include <filesystem>
namespace filesystem = std::filesystem;
#endif

#include <string>
#include <list>

#include "hresult.h"

// resolves application directory (may be different from working directory)
std::wstring Get_Application_Dir();

// appends next level to supplied path using preferred separator
std::wstring& Path_Append(std::wstring& path, const wchar_t* level);

// lists directory at supplied path; fills target list with absolute paths
template <typename A = std::allocator<std::wstring>>
std::list<std::wstring, A> List_Directory(std::wstring basePath) {

	std::list<std::wstring, A> result;

	filesystem::path base(basePath);
	if (!base.empty()) {

		// just to be sure all paths are using preferred separators
		base.make_preferred();

		for (auto& item : filesystem::directory_iterator(base))
			result.push_back(item.path().wstring());
	}

	return result;
}

