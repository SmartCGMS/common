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
 * Univerzitni 8
 * 301 00, Pilsen
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

#include "winapi_mapping.h"

#include "../iface/NetIface.h"

// Sets the socket into blocking / non-blocking state
bool Set_Socket_Blocking_State(SOCKET skt, bool state);

// Initializes network subsystem of run-time environment
bool Init_Network();

// De-initializes network subsystem of run-time environment
bool Deinit_Network();


// Helper method retrieving the fixed body part ref
template<typename T>
inline T& Get_Fixed_Body(scgms::TNet_Packet_Header* base)
{
	return *reinterpret_cast<T*>(base + 1);
}

// Helper method retrieving the dynamic body part ref
template<typename T, typename U>
inline U* Get_Dynamic_Body(scgms::TNet_Packet_Header* base)
{
	return reinterpret_cast<U*>(reinterpret_cast<T*>(base + 1) + 1);
}
