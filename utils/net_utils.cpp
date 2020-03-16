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

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

#include "net_utils.h"

bool Set_Socket_Blocking_State(SOCKET skt, bool state)
{
#ifdef WIN32
	u_long mode = state ? 1 : 0;
	auto result = ioctlsocket(skt, FIONBIO, &mode);
	if (result != NO_ERROR)
		return false;
#else
	int arg;

	if ((arg = fcntl(skt, F_GETFL, NULL)) < 0)
		return false;

	if (state)
		arg |= O_NONBLOCK;
	else
		arg &= (~O_NONBLOCK);

	if (fcntl(skt, F_SETFL, arg) < 0)
		return false;
#endif

	return true;
}

bool Init_Network()
{
#ifdef _WIN32
	// startup WSA on Windows
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(version, &data) != 0)
		return false;
#endif
	return true;
}

bool Deinit_Network()
{
#ifdef _WIN32
	// https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
	// "An application must call the WSACleanup function for every successful time the WSAStartup function is called.
	//  This means, for example, that if an application calls WSAStartup three times, it must call WSACleanup three times."
	WSACleanup();
#endif
	return true;
}