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

#define TrackLeaks	
	//Sometimes, e.g. SaveFileDialog with Qt causes hangup

#ifdef TrackLeaks

	#define prefer_vld
		//Visual Leak Detector - vld.codeplex.com
		//This will cause problems, if a dll enabling VLD is loaded in a thread that does not finish as the last one.


	#if defined(_MSC_VER) && defined(_DEBUG)
		#ifdef prefer_vld
			//#include <vld.h>
			#include "../../Third Party/Visual Leak Detector/include/vld.h"
		#else

			//The Thread Building Blocks has to include prior redefining new
			#ifdef TBB_USE_DEBUG
				#include "tbb/tbb.h"
				#include "tbb/task.h"
				#include "tbb/atomic.h"
			#endif

			#include <crtdbg.h>
			#define _CRTDBG_MAP_ALLOC
			#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
			#define new DEBUG_CLIENTBLOCK
		#endif	
	#endif

#endif

#include <sstream>


#include <iostream>
#include <fstream>	

#define overridedefdprintf

#if defined(_MSC_VER) && !defined(overridedefdprintf)
	#include <Dbgeng.h>	//provides dprintf
#else

#ifdef _WIN32
	#include <Windows.h>
#else
	#define OutputDebugStringA(x) (void)(x) // to avoid "unused variable" warning
	#define OutputDebugStringW(x) (void)(x)
	#define swprintf_s swprintf
	#define sprintf_s(x, y, z, a)
#endif

	template <typename... Args>
	void dprintf(char *format, Args... args) {
	   const size_t bufsize=2048;
	   char buf[bufsize];
	   sprintf_s(buf, bufsize, format, args...);
	   OutputDebugStringA(buf); 
	}

	template <typename... Args>
	void dprintf(wchar_t *format, Args... args) {
	   const size_t bufsize=2048;
	   wchar_t buf[bufsize];
	   swprintf_s(buf, bufsize, format, args...);
	   OutputDebugStringW(buf); 
	}

	template <typename T>
	void dprintf(T arg) {
		std::stringstream stream;
		stream << arg;		
		OutputDebugStringA(stream.str().c_str());
	}

#endif