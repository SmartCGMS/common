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


#include "system_utils.h"


#ifdef _WIN32
	#include <Windows.h>	
#endif

#ifdef _M_X64
	#include <intrin.h>
#endif

#include <iostream>
#include <array>

CPriority_Guard::CPriority_Guard() {
#ifdef _WIN32
	if (SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS))
		std::wcout << L"Process priority lowered to BELOW_NORMAL." << std::endl;
#endif
}
	
CPriority_Guard::~CPriority_Guard() {
#ifdef _WIN32		
	if (SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS))
		std::wcout << L"Process priority restored to NORMAL." << std::endl;
#endif
}


TCPU_Strings  Get_CPU_Strings() {
	

	auto append_extension = [](std::string& target, const std::string& desc, const auto set) {
		if (set != 0) {
			if (!target.empty())
				target += " ";
			target += desc;
		}
	};


#ifdef _M_X64
	std::array<int, 4> cpui;
	unsigned nExIds, i = 0;

	char cpu_brand_string[65];
	memset(cpu_brand_string, 0, sizeof(cpu_brand_string));
	cpu_brand_string[sizeof(cpu_brand_string) - 1] = '\0'; // static analysis does not detect the zeroing on the previous line

	__cpuid(cpui.data(), 0x80000000);
	nExIds = cpui[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(cpui.data(), i);
		if (i == 0x80000002)
			memcpy(cpu_brand_string, cpui.data(), sizeof(int) * cpui.size());
		else if (i == 0x80000003)
			memcpy(cpu_brand_string + 16, cpui.data(), sizeof(int) * cpui.size());
		else if (i == 0x80000004)
			memcpy(cpu_brand_string + 32, cpui.data(), sizeof(int) * cpui.size());
	}

	TCPU_Strings result;
	result.name = cpu_brand_string;

	__cpuid(cpui.data(), 0);
	nExIds = cpui[0];

	constexpr size_t EBX_idx = 1;
	constexpr size_t ECX_idx = 2;

	if (nExIds >= 1) {
		__cpuidex(cpui.data(), 1, 0); //function 1
		append_extension(result.simd_extensions, "AVX", cpui[ECX_idx] & (1 << 28));

		append_extension(result.special_instructions, "FMA", cpui[ECX_idx] & (1 << 12));
	}

	if (nExIds >= 7) {
		__cpuidex(cpui.data(), 7, 0); //function 7
		append_extension(result.simd_extensions, "AVX2", cpui[EBX_idx] & (1 << 5));

		append_extension(result.simd_extensions, "AVX512F", cpui[EBX_idx] & (1 << 16));
		append_extension(result.simd_extensions, "AVX512DQ", cpui[EBX_idx] & (1 << 17));
		append_extension(result.simd_extensions, "AVX512PF (Xeon Phi)", cpui[EBX_idx] & (1 << 26));
		append_extension(result.simd_extensions, "AVX512ER (Xeon Phi)", cpui[EBX_idx] & (1 << 27));
		append_extension(result.simd_extensions, "AVX512CD", cpui[EBX_idx] & (1 << 28));
		append_extension(result.simd_extensions, "AVX512BW", cpui[EBX_idx] & (1 << 30));
		append_extension(result.simd_extensions, "AVX512VL", cpui[EBX_idx] & (1 << 31));

		append_extension(result.simd_extensions, "AVX512_VBMI", cpui[ECX_idx] && (1 << 1));
	}

	return result;

#elif defined(_M_IX86)
	return { "A legacy 32-bit x86 processor", "unknown-simd extensions"};
#elif defined(_M_ARM)
	return {"A legacy 32-bit Arm processor", "unknown-simd extensions"};
#elif defined(_M_ARM64)
	return {"A 64-bit Arm processor", "unknown-simd extensions"};
#else
	return {"Unknown processor", "unknown-simd extensions"};
#endif
}