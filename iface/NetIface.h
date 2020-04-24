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

#include <cstdint>
#include "../rtl/guid.h"

namespace scgms
{

#pragma pack(push, 1)

	// protocol magic used as a first field in header
	constexpr uint32_t ProtoMagic = 0x5343474D; // ASCII: 'SCGM'

	// protocol (major) version used in handshake - mismatch would cause fatal error
	constexpr uint16_t ProtoVersionMajor = 0x0001;
	// protocol (minor) version used in handshake - mismatch would cause warning
	constexpr uint16_t ProtoVersionMinor = 0x0003;

	// maximum total length of a packet (header + body), aka buffer size
	constexpr size_t Max_Packet_Length = 1024;

	// invalid session ID (negate zero - defined behaviour on unsigned types)
	constexpr uint64_t Invalid_Session_Id = ~(static_cast<uint64_t>(0));

	// maximum length of supported model name (and total fixed size of name field)
	constexpr size_t Model_Short_Name_Length = 32;

	/*
	 * Enumeration of status codes send through network
	 */
	enum class NNet_Status_Code : uint8_t
	{
		OK					= 0,
		FAIL_VERSION		= 1,	// major version mismatch
		FAIL_UNK_SESSION	= 2,	// unknown session requested to be restored
		FAIL_UNK_MODEL		= 3,	// unknown model requested
		FAIL_NO_SLOT		= 4,	// no slot available within selected model
	};

	/*
	 * Enumeration of possible synchronization sources:
	 *
	 * active model - sends the first signal set and requests shutdown (e.g.; maintains the total simulation duration)
	 * passive model - sends the signal set only as a response to active model signal set, expects shutdown from remote side
	 */
	enum class NNet_Sync_Source : uint8_t
	{
		SMARTCGMS			= 0,	// SmartCGMS - active,  external - passive
		EXTERNAL			= 1,	// SmartCGMS - passive, external - active
	};

	/*
	 * Opcodes enumeration:
	 *
	 * Opcode naming recognizes two sides and one special category:
	 *	1) SmartCGMS (coded as "S")
	 *	2) external model (coded as "E")
	 *  3) unspecified/unknown (coded as "U")
	 * the opcode name contains coded direction as a prefix; the first letter is source, the second letter is destination
	 * e.g.; SE_TESTPKT is a packet sent from SmartCGMS to external model named "TESTPKT"
	 */
	enum class NOpcodes : uint8_t
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

		// SmartCGMS advances model and sends data to the external model
		SE_ADVANCE_MODEL = 0x05,

		// external model advances and sends data to SmartCGMS
		ES_ADVANCE_MODEL = 0x06,

		// SmartCGMS requests a list of supported models
		SE_ENUMERATE_MODELS_REQUEST = 0x07,

		// external model controller sends a list of supported models
		ES_ENUMERATE_MODELS_REPLY = 0x08,

		// miscellanous, control packets

		// keepalive request - any side requests confirmation, that the connection is still alive and session state is still consistent
		UU_KEEPALIVE_REQUEST = 0xF0,

		// keepalive reply - the requested side confirms, that the connection is still alive and session state is still consistent
		UU_KEEPALIVE_REPLY = 0xF1,
	};

	/*
	 * Every packet header
	 */
	struct TNet_Packet_Header
	{
		uint32_t magic;		// 4B, always == scgms::ProtoMagic
		NOpcodes opcode;	// 1B, operation code, always valid
		uint8_t flags;		// 1B, unused for now, used as a placeholder and a padding to a multiple of 4 bytes
		uint16_t length;	// 2B, total length of header + body
	};

	/*
	 * Packet SE_HANDSHAKE_REQUEST structure
	 */
	struct TPacket_Handshake_Request_Body
	{
		uint16_t protocol_version_major;	// client reports his protocol major version; this version must match the remote one
		uint16_t protocol_version_minor;	// client reports his protocol minor version; this version may not match the remote one, but possible differences are easier to identify thanks to this
		uint64_t session_id;				// if the connection was closed, restore session using pre-shared session ID (previous handshakes); use scgms::Invalid_Session_Id to indicate no session should be restored
		GUID session_secret;				// a valid session secret generated by client; this must match when attempting to restore session
		double tick_interval;				// interval in RatTime
		GUID requested_model_id;			// GUID of the requested model; if session_id != scgms::Invalid_Session_Id, this field is ignored
	};

	/*
	 * Packet ES_HANDSHAKE_REPLY structure
	 */
	struct TPacket_Handshake_Reply_Body
	{
		NNet_Status_Code status;			// accepts or declines the client request based on reported version
		uint64_t session_id;				// remote-side assigned session ID (may be set to 0 if the remote side does not support session management)
		NNet_Sync_Source sync_source;		// source of synchronization - the side written here is the one which requests the synchronization, i.e.; is "authoritative" - this is decided by the external model as SmartCGMS knows how to behave in both modes
		uint8_t requested_signal_count;		// number of TNet_Requested_Signal_Item items that follow
		// dynamic data: requested_signal_count * TNet_Requested_Signal_Item
	};

	/*
	 * Enumeration (weakly typed) of possible signal flags
	 */
	enum NNetmodel_Signal_Flags : uint8_t
	{
		Netmodel_Signal_Flag_Requires_Fresh_Value = 0x01,	// always require a fresh value of given signal; otherwise use last available
		Netmodel_Signal_Flag_Send_Only_Fresh_Value = 0x02,	// if a fresh value is available, send it; otherwise don't send it - this is valid for e.g.; some event-based signals
		Netmodel_Signal_Flag_Allow_Multiple_Values = 0x04,	// allow multiple values of a given signal to be sent
	};

	/*
	 * Dynamic part of ES_HANDSHAKE_REPLY packet
	 */
	struct TNet_Requested_Signal_Item
	{
		GUID signal_id;						// GUID of requested signal
		uint8_t flags;						// signal flags (Netmodel_Signal_Flags flag field)
	};

	/*
	 * Packets SE_ADVANCE_MODEL and ES_ADVANCE_MODEL structure
	 */
	struct TPacket_Advance_Model_Body
	{
		double device_time;					// current RatTime of the running simulation
		uint8_t signal_count;				// number of following TNet_Data_Item items
		// dynamic data: signal_count * TNet_Data_Item
	};

	/*
	 * Dynamic part of SE_ADVANCE_MODEL and ES_ADVANCE_MODEL packets
	 */
	struct TNet_Data_Item
	{
		GUID signal_id;						// GUID of signal
		double device_time;					// RatTime of emitted signal
		double level;						// level of emitted signal
	};

	/*
	 * Packet ES_ENUMERATE_MODELS_REPLY structure
	 */
	struct TPacket_Enumerate_Models_Reply_Body
	{
		uint8_t model_count;				// number of following TNet_Model_List_Item items
		// dynamic data: model_count * TNet_Model_List_Item
	};

	/*
	 * Dynamic part of ES_ENUMERATE_MODELS_REPLY packet
	 */
	struct TNet_Model_List_Item
	{
		GUID model_id;							// GUID of a supported model
		wchar_t name[Model_Short_Name_Length];	// model name of a fixed width (Model_Short_Name_Length), always zero-terminated
	};

	/*
	 * Helper structure of a network packet - generic packet with type(-T)-defined body
	 */
	template<typename T>
	struct TNet_Packet
	{
		scgms::TNet_Packet_Header header;
		T body;

		TNet_Packet(NOpcodes opcode, size_t length = sizeof(T)) {
			header.magic = ProtoMagic;
			header.opcode = opcode;
			header.flags = 0;
			header.length = sizeof(header) + static_cast<decltype(header.length)>(length);
		}
	};

	/*
	 * Helper structure of a network packet without a body
	 * Explicit specialization of TNet_Packet<T>
	 */
	template<>
	struct TNet_Packet<void>
	{
		scgms::TNet_Packet_Header header;

		TNet_Packet(NOpcodes opcode, size_t length = 0) {
			header.magic = ProtoMagic;
			header.opcode = opcode;
			header.flags = 0;
			header.length = sizeof(header) + static_cast<decltype(header.length)>(length);
		}
	};

#pragma pack(pop)

}
