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

#pragma once

#include <cstdint>
#include <functional>

#include "../rtl/guid.h"
#include "referencedIface.h"

#include <cstdint>
#include <limits>

namespace scgms
{

#pragma pack(push, 1)

	// protocol magic used as a first field in header
	constexpr uint32_t ProtoMagic = 0x5343474D; // ASCII: 'SCGM'

	// protocol (major) version used in handshake - mismatch would cause fatal error
	constexpr uint16_t ProtoVersionMajor = 0x0001;
	// protocol (minor) version used in handshake - mismatch would cause warning
	constexpr uint16_t ProtoVersionMinor = 0x0005;

	// maximum total length of a packet (header + body), aka buffer size
	constexpr size_t Max_Packet_Length = 1024;

	// invalid session ID (negate zero - defined behaviour on unsigned types)
	constexpr uint64_t Invalid_Session_Id = ~(static_cast<uint64_t>(0));

	// maximum length of supported model name (and total fixed size of name field)
	constexpr size_t Model_Short_Name_Length = 32;

	// maximum length of subject name (and total fixed size of subject name field)
	constexpr size_t Subject_Name_Length = 64;

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
		uint32_t magic = scgms::ProtoMagic;		// 4B, always == scgms::ProtoMagic
		NOpcodes opcode = NOpcodes::UU_NONE;	// 1B, operation code, always valid
		uint8_t flags = 0;		// 1B, unused for now, used as a placeholder and a padding to a multiple of 4 bytes
		uint16_t length = 0;	// 2B, total length of header + body
	};

	/*
	 * Packet SE_HANDSHAKE_REQUEST structure
	 */
	struct TPacket_Handshake_Request_Body
	{
		uint16_t protocol_version_major = 0;									// client reports his protocol major version; this version must match the remote one
		uint16_t protocol_version_minor = 0;									// client reports his protocol minor version; this version may not match the remote one, but possible differences are easier to identify thanks to this
		uint64_t session_id = 0;												// if the connection was closed, restore session using pre-shared session ID (previous handshakes); use scgms::Invalid_Session_Id to indicate no session should be restored
		GUID session_secret = Invalid_GUID;										// a valid session secret generated by client; this must match when attempting to restore session
		double tick_interval = std::numeric_limits<double>::quiet_NaN();		// interval in RatTime
		GUID requested_model_id = Invalid_GUID;									// GUID of the requested model; if session_id != scgms::Invalid_Session_Id, this field is ignored
		wchar_t subject_name[Subject_Name_Length] = {};							// null-terminated subject name, empty (zero-filled) if not chosen or requested
		double initial_time = std::numeric_limits<double>::quiet_NaN();			// rat time at the beginning of simulation; zero if the initiating side is not the synchronization source
		double requested_duration = std::numeric_limits<double>::quiet_NaN();	// requested rat time duration of simulation; zero if the initiating side is not the synchronization source or if no request made
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
		GUID signal_id =  Invalid_GUID;						// GUID of requested signal
		uint8_t flags = 0;						// signal flags (Netmodel_Signal_Flags flag field)
	};

	/*
	 * Packets SE_ADVANCE_MODEL and ES_ADVANCE_MODEL structure
	 */
	struct TPacket_Advance_Model_Body
	{
		double device_time = std::numeric_limits<double>::quiet_NaN();					// current RatTime relative to the start of the simulation
		uint8_t signal_count = 0;				// number of following TNet_Data_Item items
		// dynamic data: signal_count * TNet_Data_Item
	};

	/*
	 * Dynamic part of SE_ADVANCE_MODEL and ES_ADVANCE_MODEL packets
	 */
	struct TNet_Data_Item
	{
		GUID signal_id = Invalid_GUID;									// GUID of signal
		double device_time = std::numeric_limits<double>::quiet_NaN();	// RatTime of emitted signal
		double level = std::numeric_limits<double>::quiet_NaN();		// level of emitted signal
	};

	/*
	 * Packet ES_ENUMERATE_MODELS_REPLY structure
	 */
	struct TPacket_Enumerate_Models_Reply_Body
	{
		uint8_t model_count = 0;				// number of following TNet_Model_List_Item items
		// dynamic data: model_count * TNet_Model_List_Item
	};

	/*
	 * Dynamic part of ES_ENUMERATE_MODELS_REPLY packet
	 */
	struct TNet_Model_List_Item
	{
		GUID model_id = Invalid_GUID;							// GUID of a supported model
		wchar_t name[Model_Short_Name_Length] = {};	// model name of a fixed width (Model_Short_Name_Length), always zero-terminated
	};

	/*
	 * Helper structure of a network packet - generic packet with type(-T)-defined body
	 */
	template<typename T>
	struct TNet_Packet
	{
		scgms::TNet_Packet_Header header;
		T body{};

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

	/*
	 * Namespace designated to hold classes and structures relevant to distributed generic subsystem of SmartCGMS
	 */
	namespace forum {

		enum class NTransfer_File_Type : uint8_t {
			Filters_Library,
			Misc_File,
		};

		/*
		 * Class representing a running solver instance on a node within the network; this usually implies a dependence on
		 * the INet_Worker class instance, as it uses its services to interact with other solver instances
		 */
		class INet_Solver : public virtual refcnt::IReferenced {
			public:
				/*
				 * broadcast a candidate solution to all other nodes; the caller manages the parameters container lifetime
				 * returns S_OK if the broadcast was done successfully
				 */
				virtual HRESULT Broadcast_Candidate(refcnt::double_container* parameters, uint32_t generation, GUID* source_id) = 0;

				/*
				 * determines if there is a foreign candidate waiting in the queue
				 * returns S_OK if there is at least one candidate waiting
				 * returns S_FALSE if the queue is empty
				 */
				virtual HRESULT Has_Foreign_Candidate() = 0;

				/*
				 * pops a foreign candidate from the queue, fills the caller supplied parameters, generation and source_id pointers
				 * the caller manages lifetime of all supplied referenced entities
				 * returns S_OK if the request was successfull
				 * returns E_FAIL when there is no candidate waiting in the queue
				 */
				virtual HRESULT Pop_Foreign_Candidate(refcnt::double_container* parameters, uint32_t* generation, GUID* source_id) = 0;

				/*
				 * finalizes the communication; this call blocks until the whole forum is finalized (every INet_Solver calls Finalize)
				 * results S_OK if the finalization was successfull
				 */
				virtual HRESULT Finalize() = 0;

				/*
				 * broadcast a best solution to all other nodes; the caller manages the parameters container lifetime
				 * returns S_OK if the broadcast was done successfully
				 */
				virtual HRESULT Broadcast_Best(refcnt::double_container* parameters, uint32_t generation, GUID* source_id) = 0;

				/*
				 * determines if there is a foreign best solution waiting in the queue
				 * returns S_OK if there is at least one best solution waiting
				 * returns S_FALSE if the queue is empty
				 */
				virtual HRESULT Has_Foreign_Best() = 0;

				/*
				 * pops a foreign candidate from the queue, fills the caller supplied parameters and source_id pointers
				 * the caller manages lifetime of all supplied referenced entities
				 * returns S_OK if the request was successfull
				 * returns E_FAIL when there is no best solution waiting in the queue
				 */
				virtual HRESULT Pop_Foreign_Best(refcnt::double_container* parameters, GUID* source_id) = 0;
		};

		using TActivate_File_Fnc = HRESULT(*)(void* /*data*/, refcnt::wstr_container* /*file_name*/, refcnt::wstr_container* /*version_identifier*/);

		using TUpdate_File_Fnc = HRESULT(*)(void* /*data*/, refcnt::wstr_container* /*file_name*/, refcnt::wstr_container* /*version_identifier*/, refcnt::IVector_Container<uint8_t>* /*file_contents*/);

		using TOptimize_Request_Fnc = HRESULT(*)(void* /*data*/, refcnt::wstr_container* /*config_name*/, uint32_t /*filter_idx*/, refcnt::wstr_container* /*parameters_name*/, refcnt::double_container* /*result*/, INet_Solver* /*solver_comm*/);

		/*
		 * Class representing a worker in a network; this class maintains all the control and service purposes
		 * This is usually visible only to the front-end; the class implementing INet_Worker should use an underlying
		 * technology, such as MPI (acting as a non-root node), to exchange info between nodes
		 */
		class INet_Worker : public virtual refcnt::IReferenced {
			public:
				/*
				 * starts the network worker code - this opens a given implementation-specific communication channel and begins listening for requests
				 * returns S_OK if the worker is ready
				 */
				virtual HRESULT Start() = 0;

				/*
				 * finalizes the network code, closes the communication channel and exits; it either does so gracefully (force == FALSE) or forced
				 * returns S_OK if the network was gracefully finalized
				 * returns S_FALSE if force == TRUE and forced shut-down was required
				 */
				virtual HRESULT Stop(BOOL force) = 0;

				/*
				 * registers an activate file listener function; this function gets called when activate file request is obtained through the network interface
				 * returns S_OK if the function was registered successfully and no other function was registered prior
				 * returns S_FALSE if the registration has overwritten a previously registered function
				 */
				virtual HRESULT Register_Activate_File_Listener(TActivate_File_Fnc handler, void* handler_data) = 0;

				/*
				 * registers an update file listener function; this function gets called when update file request bulk is obtained through the network interface (whole file is received)
				 * returns S_OK if the function was registered successfully and no other function was registered prior
				 * returns S_FALSE if the registration has overwritten a previously registered function
				 */
				virtual HRESULT Register_Update_File_Listener(TUpdate_File_Fnc handler, void* handler_data) = 0;

				/*
				 * registers an optimize listener function; this function gets called when Optimize request is obtained through the network interface
				 * returns S_OK if the function was registered successfully and no other function was registered prior
				 * returns S_FALSE if the registration has overwritten a previously registered function
				 */
				virtual HRESULT Register_Optimize_Listener(TOptimize_Request_Fnc handler, void* handler_data) = 0;
		};

		/*
		 * Class representing a coordinator of network cluster consisting of one or more workers; this class maintains
		 * all the control and service purposes
		 * This is usually visible only to the front-end; the class implementing INet_Coordinator should use an underlying
		 * technology, such as MPI (acting as a root node), to exchange info between nodes
		 */
		class INet_Coordinator : public virtual refcnt::IReferenced {
			public:
				/*
				 * connect to all available workers in a pool up to a worker_count - this allocates all such workers to work with this instance; actual number of workers are returned in worker_count
				 * if *worker_count == 0, all available workers are used; *worker_count is filled with the actual number of workers allocated
				 * returns S_OK if all requested workers (or at least 1 if *worker_count == 0) were allocated, S_FALSE if at least 1 and < *worker_count was allocated, E_FAIL when no workers are available
				 */
				virtual HRESULT Connect_Workers(size_t* worker_count) = 0;

				/*
				 * collects worker info - fills in the worker count and environment specs
				 * envSpecs is an array of size worker_count
				 * returns S_OK if all workers responded with required data
				 */
				virtual HRESULT Collect_Worker_Info(size_t* worker_count, refcnt::IVector_Container<refcnt::wstr_container*>** envSpecs) = 0;

				/*
				 * distributes a file of a given file class/type to workers; the caller must supply full path to the file and a version identifier (e.g., a hash)
				 * the callee queries all workers of their current version and updates the given file if neccessary; the worker activates given file during this request
				 * env_specifier specifies the environment, if applies (e.g., Windows, Linux, ...)
				 * returns S_OK if the operation succeeded and at least one worker was updated
				 * returns S_FALSE when no worker was updated due to already having the requested version
				 * returns an error code when failure occurred
				 */
				virtual HRESULT Update_File_In_Workers(NTransfer_File_Type file_type, refcnt::wstr_container* file_path, refcnt::wstr_container* env_specifier, refcnt::wstr_container* file_version_identifier, refcnt::IVector_Container<uint8_t>* file_contents) = 0;

				/*
				 * runs an optimize job on all connected workers, so that they optimize given config name (distributed prior this call using Update_File_In_Workers call), its filter with index of filter_idx
				 * and its parameters identified by parameters_name. As a result, it sets the worker_count to an actual number of workers that responded with valid response. This also serves as a size of the
				 * following parameters array; the parameters container array contains the optimized parameters from each worker
				 * 
				 * NOTE: this does not pose any requirement for inter-solver communication, as it does not pass through the coordinator interface. The implementation of data passing between solvers is
				 * an implementation detail of INet_Coordinator, as it merely forwards the messages to other workers and their linked INet_Solver instances
				 * 
				 * returns S_OK if all remote solvers returned a valid parameters array
				 * returns S_FALSE if at least one solver returned a valid parameters array, but at least one failed to do so
				 * returns E_ABORT when no solvers were able to return a valid parameters array (but they were successfully prompted) - this indicates an error in configuration
				 * returns E_FAIL when there is a network error
				 */
				virtual HRESULT Optimize(refcnt::wstr_container* config_name, GUID* solver_id, uint32_t filter_idx, refcnt::wstr_container* parameters_name, uint32_t* worker_count, refcnt::double_container* parameters) = 0;
		};

	}

}
