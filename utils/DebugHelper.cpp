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

#include "DebugHelper.h"

#if defined(_MSC_VER) && defined(_DEBUG)
    #include <wchar.h>
	#include <stdlib.h>	
	#include <Windows.h>

    extern "C" char __ImageBase;
#endif

	

class CMemory_Leak_Tracker {
	//just a class-helper that correctly catches memory allocated with global const variable
public:
	CMemory_Leak_Tracker();
	~CMemory_Leak_Tracker();
};

extern const CMemory_Leak_Tracker Memory_Leak_Tracker;

	

CMemory_Leak_Tracker::CMemory_Leak_Tracker() {
	#ifdef TrackLeaks
		#if defined(_MSC_VER) && defined(_DEBUG)
			#ifdef prefer_vld
				VLDEnable();
			#else
				_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
			#endif
		#endif
	#endif
}

CMemory_Leak_Tracker::~CMemory_Leak_Tracker() {
	#ifdef TrackLeaks
		#if defined(_MSC_VER) && defined(_DEBUG)
	
			//wchar_t *fileName = (wchar_t*) malloc(1024*sizeof(wchar_t));
			//wchar_t *buf = (wchar_t*) malloc(2048*sizeof(wchar_t));
			//can't be on the heap, otherwise they will be reported as leaks
			wchar_t fileName[1024];
			wchar_t buf[2048];
			GetModuleFileNameW(((HINSTANCE)&__ImageBase), fileName, 1024);

			swprintf_s(buf, 2048, L"========== Dumping leaks for: %s ==========\n", fileName);
			OutputDebugStringW(buf);

			#ifdef prefer_vld
				VLDReportLeaks();
			#else
				_CrtDumpMemoryLeaks();
			#endif

			swprintf_s(buf, 2048, L"========== Leaks dumped for: %s ==========\n", fileName);
			OutputDebugStringW(buf);


		#endif
	#endif
}

const CMemory_Leak_Tracker Memory_Leak_Tracker;