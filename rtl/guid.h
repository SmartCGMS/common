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

#include <assert.h>

#ifdef _WIN32
	#include <guiddef.h>
#else
	#ifdef __cplusplus
		#include <cstring>
		#include <cstdint>
	#else
		#include <string.h>
		#include <stdint.h>
	#endif

	typedef struct GUID {
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t  Data4[8];
	} GUID;

	// some libraries expect the constant GUID_DEFINED to be defined in order to avoid redefinition of GUID structure
	#define GUID_DEFINED

	#ifdef __cplusplus
		static inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2) {
			return !memcmp(&rguid1, &rguid2, sizeof(GUID));
		}

		static inline bool operator==(const GUID& guidOne, const GUID& guidOther) {
			return !!IsEqualGUID(guidOne, guidOther);
		}

		static inline bool operator!=(const GUID& guidOne, const GUID& guidOther) {
			return !(guidOne == guidOther);
		}
	#else
		static inline int IsEqualGUID(const GUID *rguid1, const GUID *rguid2) {
			return !memcmp(rguid1, rguid2, sizeof(GUID));
		}
	#endif
#endif

#ifdef __cplusplus
	static inline bool operator<(const GUID& rguid1, const GUID& rguid2) {
		return memcmp(&rguid1, &rguid2, sizeof(GUID)) < 0;
	}
	constexpr GUID Invalid_GUID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

	static inline bool Is_Invalid_GUID(const GUID& id) {
		return id == Invalid_GUID;
	}

	template <typename... Args>
	bool Is_Invalid_GUID(const GUID& id, const Args&... args) {
		return Is_Invalid_GUID(id) || Is_Invalid_GUID(args...);
	}


	// on several systems (OS's and compilers), the lengths of standard types may vary (e.g. "unsigned long" on LLP64 vs. LP64);
	// the C++ standard itself defines minimal length, but does not guarantee exact length on every platform;
	// this is here to ensure correct lengths of all GUID fields

	static_assert(sizeof(GUID::Data1) == 4, "GUID Data1 (unsigned long) is not 4 bytes long");
	static_assert(sizeof(GUID::Data2) == 2, "GUID Data2 (unsigned short) is not 2 bytes long");
	static_assert(sizeof(GUID::Data3) == 2, "GUID Data3 is not 2 bytes long");
	static_assert(sizeof(GUID::Data4) == 8, "GUID Data4 is not 8 bytes long");
#endif

// Generates a new GUID version 4 (completely random, suitable for network traffic)
GUID Generate_GUIDv4();
