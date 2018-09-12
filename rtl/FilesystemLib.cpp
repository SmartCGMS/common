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

#include "FilesystemLib.h"


#include "hresult.h"
#include "../utils/winapi_mapping.h"

#include <cstring>

std::wstring Get_Application_Dir() {

	const size_t bufsize = 1024;

#ifdef _WIN32
	wchar_t ModuleFileName[bufsize];
	GetModuleFileNameW(((HINSTANCE)&__ImageBase), ModuleFileName, bufsize);
#else
	char ModuleFileName[bufsize];
	memset(ModuleFileName, 0, bufsize);
	readlink("/proc/self/exe", ModuleFileName, bufsize);
	// TODO: error checking
#endif


#ifdef DHAS_FILESYSTEM
	filesystem::path exePath { ModuleFileName};

	return exePath.remove_filename().wstring();
#else
	std::string spath(ModuleFileName);
	std::wstring path(spath.begin(), spath.end());

	size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
		path = path.substr(0, pos + 1);

	return path;
#endif
}

std::wstring& Path_Append(std::wstring& path, const wchar_t* level) {
#ifdef DHAS_FILESYSTEM
	// use overloaded operator/, which uses preferred path component separator
	path = (filesystem::path(path) / level).wstring();
#else
	if (path.substr(path.size() - 1, 1) != L"/")
		path += L"/";
	path += level;
#endif
	return path;
}

