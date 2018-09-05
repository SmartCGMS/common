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

#ifdef _WIN32
	#include <guiddef.h>
#else
	#include <cstring>
	#include <cstdint>

	struct GUID {
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t  Data4[8];
	};

	// some libraries expect the constant GUID_DEFINED to be defined in order to avoid redefinition of GUID structure
	#define GUID_DEFINED

	inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
	{
		return !memcmp(&rguid1, &rguid2, sizeof(GUID));
	}

	inline bool operator==(const GUID& guidOne, const GUID& guidOther)
	{
		return !!IsEqualGUID(guidOne, guidOther);
	}

	inline bool operator!=(const GUID& guidOne, const GUID& guidOther)
	{
		return !(guidOne == guidOther);
	}
#endif

constexpr GUID Invalid_GUID{ 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

inline bool operator<(const GUID& rguid1, const GUID& rguid2)
{
	return memcmp(&rguid1, &rguid2, sizeof(GUID)) < 0;
}

// on several systems (OS's and compilers), the lengths of standard types may vary (e.g. "unsigned long" on LLP64 vs. LP64);
// the C++ standard itself defines minimal length, but does not guarantee exact length on every platform;
// this is here to ensure correct lengths of all GUID fields

static_assert(sizeof(GUID::Data1) == 4, "GUID Data1 (unsigned long) is not 4 bytes long");
static_assert(sizeof(GUID::Data2) == 2, "GUID Data2 (unsigned short) is not 2 bytes long");
static_assert(sizeof(GUID::Data3) == 2, "GUID Data3 is not 2 bytes long");
static_assert(sizeof(GUID::Data4) == 8, "GUID Data4 is not 8 bytes long");
