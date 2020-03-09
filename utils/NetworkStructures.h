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

#include <cstdint>
#include "../rtl/guid.h"

namespace scgms
{

#pragma pack(push, 1)

	// protocol magic used in header
	constexpr uint32_t ProtoMagic = 0x5343474D; // ASCII: 'SCGM'

	// protocol (major) version used in handshake
	constexpr uint16_t ProtoVersionMajor = 0x0001;
	// protocol (minor) version used in handshake
	constexpr uint16_t ProtoVersionMinor = 0x0001;

	// maximum total length of a packet (header + body) aka buffer size
	constexpr size_t Max_Packet_Length = 1024;

	// invalid session ID
	constexpr uint32_t Invalid_Session_Id = ~(static_cast<uint32_t>(0));

	enum class Net_Status_Code : uint8_t
	{
		OK					= 0,
		FAIL_VERSION		= 1,
		FAIL_UNK_SESSION	= 2,
	};

	enum class Net_Sync_Source : uint8_t
	{
		SMARTCGMS			= 0,	// SmartCGMS - active,  external - passive
		EXTERNAL			= 1,	// SmartCGMS - passive, external - active
	};

	/* Opcode naming recognizes two sides:
	 *	1) SmartCGMS (coded as "S")
	 *	2) external model (coded as "E")
	 *  3) unspecified/unknown (coded as "U")
	 * the opcode name contains coded direction as a prefix; the first letter is source, the second letter is destination
	 * e.g.; SE_TESTPKT is a packet sent from SmartCGMS to external model named "TESTPKT"
	 */

	enum class Opcodes : uint8_t
	{
		// dummy, empty
		UU_NONE = 0,

		// SmartCGMS requests handshake; this may serve as a session restoring mechanism
		SE_HANDSHAKE_REQUEST = 0x01,

		// external model confirms the handshake request receipt and sends a list of signals of interest
		ES_HANDSHAKE_REPLY = 0x02,

		// any side may initiate session teardown - this effectively cleans the session state and deallocates associated resources
		UU_TEARDOWN_REQUEST = 0x03,

		// the opposite side confirms session teardown and immediatelly closes the connection (in order to destroy the session)
		UU_TEARDOWN_REPLY = 0x04,

		// SmartCGMS sends data to the external model
		SE_DATA = 0x05,

		// external model sends data to SmartCGMS
		ES_DATA = 0x06,

		// miscellanous, control packets

		// ping request - any side requests confirmation, that the connection is still alive and session state is still consistent
		UU_PING_REQUEST = 0xF0,

		// ping reply - the requested side confirms, that the connection is still alive and session state is still consistent
		UU_PING_REPLY = 0xF1,
	};

	struct Net_Packet_Header
	{
		uint32_t magic;		// 4B, always == scgms::ProtoMagic
		Opcodes opcode;		// 1B, operation code, always valid
		uint8_t flags;		// 1B, unused for now
		uint16_t length;	// 2B, total length of header + body
	};

	struct Packet_Handshake_Request
	{
		uint16_t protocol_version_major;	// client reports his protocol major version; this version must match the remote one
		uint16_t protocol_version_minor;	// client reports his protocol minor version; this version may not match the remote one, but possible differences are easier to identify thanks to this
		uint32_t session_id;				// if the connection was closed, restore session using pre-shared session ID (previous handshakes); use scgms::Invalid_Session_Id to indicate no session should be restored
		double tick_interval;				// interval in RatTime
	};

	struct Packet_Handshake_Reply
	{
		Net_Status_Code status;				// accepts or declines the client request based on reported version
		uint32_t session_id;				// remote-side assigned session ID (may be set to 0 if the remote side does not support session management)
		Net_Sync_Source sync_source;		// source of synchronization - the side written here is the one which requests the synchronization, i.e.; is "authoritative" - this is decided by the external model as SmartCGMS knows how to behave in both modes
		uint8_t requested_signal_count;		// number of signal GUIDs that follow
		// dynamic data: requested_signal_count * Net_Requested_Signal_Item
	};

	struct Net_Requested_Signal_Item
	{
		GUID signal_id;
	};

	struct Packet_Data
	{
		double current_device_time;
		uint8_t signal_count;
		// dynamic data: signal_count * Net_Data_Item
	};

	struct Net_Data_Item
	{
		GUID signal_id;
		double device_time;
		double level;
	};

	template<typename T>
	struct Net_Packet
	{
		scgms::Net_Packet_Header header;
		T body;

		Net_Packet(Opcodes opcode, size_t length = sizeof(T)) {
			header.magic = ProtoMagic;
			header.opcode = opcode;
			header.flags = 0;
			header.length = length;
		}
	};

	template<>
	struct Net_Packet<void>
	{
		scgms::Net_Packet_Header header;

		Net_Packet(Opcodes opcode, size_t length = 0) {
			header.magic = ProtoMagic;
			header.opcode = opcode;
			header.flags = 0;
			header.length = length;
		}
	};

#pragma pack(pop)

}
