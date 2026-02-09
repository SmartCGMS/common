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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#include "guid.h"

#include <random>
#include <array>
#include <cstdint>

#ifdef _WIN32
	#include <Rpc.h>
#endif 


GUID Generate_GUIDv4_Rand() {

	GUID guid = Invalid_GUID;

	static std::random_device rdev;

	guid.Data1 = static_cast<decltype(guid.Data1)>(rdev());
	guid.Data2 = static_cast<decltype(guid.Data2)>(rdev());
	guid.Data3 = static_cast<decltype(guid.Data3)>(rdev());

	for (size_t i = 0; i < 8; i++) {
		guid.Data4[i] = static_cast<std::remove_reference_t<decltype(guid.Data4[0])>>(rdev());
	}

	//got it random, make it to adhere to version 4
	//encode GUID version - let's encode just the two most significant bits=>variant 1
	const uint16_t masked_version_bits = guid.Data3 & 0x0FFF;
	guid.Data3 = masked_version_bits | 0x4000;

	//encode the variant 1, i.e., just two most significant bits
	const std::array<uint8_t, 4> variant_bits = { 0x80, 0x90, 0xA0, 0xB0 };
	const uint8_t variant_sign = variant_bits[static_cast<uint8_t>(rdev()) % variant_bits.size()];
	const uint8_t masked_data4_0 = guid.Data4[0] & 0x0F;
	guid.Data4[0] = masked_data4_0 | variant_sign;

	return guid;
}

#include <iostream>

GUID Generate_GUID() {
	//when possible, let's try to prefer version 7 instead of version 4
	#ifdef _WIN32	
		GUID result = Invalid_GUID;
		if (UuidCreate(&result) != RPC_S_OK)
			result = Generate_GUIDv4_Rand();
		return result;
	#else
		return Generate_GUIDv4_Rand();
	#endif
}