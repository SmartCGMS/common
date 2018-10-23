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
 *       monitoring", Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 */

#pragma once

#include "../rtl/guid.h"
#include "referencedIface.h"

#include <limits>
#include <array>

#undef max

namespace glucose {

	//constant to convert mg/dl to mmol/l
	extern const double mgdl_2_mmoll;	//we assume mmol/l, so to make a conversion possible

	extern const double One_Hour;
	extern const double One_Minute;
	extern const double One_Second;

	const size_t apxNo_Derivation = 0;
	const size_t apxFirst_Order_Derivation = 1;

	/*
		The time is encoded as the number of days since January 0, 1900 00:00 UTC, see
		http://en.wikipedia.org/wiki/January_0

		Integral part stores the number of days, fractional part stores the time.
		It could have been any fixed dates, but this one is compatible with
		FreePascal, Delphi and Microsoft Products such as Excel, Access and COM's variant in general.

		Therefore, 01 Jan 1900 00:00 would be 1.0 and 01 Jan 1900 24:00 would be 2.0

		However, the UI is supposed to use QDateTime whose epoch starts on 1.1. 1970 0:0 UTC0 aka UNIX epoch start.
		But note that leap seconds are not calculated with when using the UNIX epoch!
	*/

	// special GUID value that means "all signal ids" - it is used especially in modules, which are able to work with a set of signals
	static constexpr GUID signal_All = { 0xffffffff, 0xffff, 0xffff, { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };	// {FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}

	static constexpr GUID signal_BG = { 0xf666f6c2, 0xd7c0, 0x43e8,{ 0x8e, 0xe1, 0xc8, 0xca, 0xa8, 0xf8, 0x60, 0xe5 } };	// {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}
	static constexpr GUID signal_IG = { 0x3034568d, 0xf498, 0x455b,{ 0xac, 0x6a, 0xbc, 0xf3, 0x1, 0xf6, 0x9c, 0x9e } };		// {3034568D-F498-455B-AC6A-BCF301F69C9E}
	static constexpr GUID signal_ISIG = { 0x3f62c28a, 0x4d25, 0x4086,{ 0xbd, 0x1a, 0xfc, 0x44, 0x2f, 0xdd, 0xb7, 0xcf } };		// {3F62C28A-4D25-4086-BD1A-FC442FDDB7CF}
	static constexpr GUID signal_Calibration = { 0xed4cd0f5, 0xf728, 0x44fe,{ 0x95, 0x52, 0x97, 0x33, 0x8b, 0xd7, 0xe8, 0xd5 } };	// {ED4CD0F5-F728-44FE-9552-97338BD7E8D5}
	static constexpr GUID signal_Insulin = { 0x22d87566, 0xaf1b, 0x4cc7,{ 0x8d, 0x11, 0xc5, 0xe0, 0x4e, 0x1e, 0x9c, 0x8a } }; 	 // {22D87566-AF1B-4CC7-8D11-C5E04E1E9C8A}
	static constexpr GUID signal_Carb_Intake = { 0x37aa6ac1, 0x6984, 0x4a06,{ 0x92, 0xcc, 0xa6, 0x60, 0x11, 0xd, 0xd, 0xc7 } };	// {37AA6AC1-6984-4A06-92CC-A660110D0DC7}
	static constexpr GUID signal_Health_Stress = { 0xf4438e9a, 0xdd52, 0x45bd,{ 0x83, 0xce, 0x5e, 0x93, 0x61, 0x5e, 0x62, 0xbd } }; // {F4438E9A-DD52-45BD-83CE-5E93615E62BD}

	//known calculated signals to allow optimizations (and localizations)
	static constexpr GUID signal_Diffusion_v2_Blood = { 0xd96a559b, 0xe247, 0x41e0,{ 0xbd, 0x8e, 0x78, 0x8d, 0x20, 0xdb, 0x9a, 0x70 } }; // {D96A559B-E247-41E0-BD8E-788D20DB9A70}
	static constexpr GUID signal_Diffusion_v2_Ist = { 0x870ddbd6, 0xdaf1, 0x4877,{ 0xa8, 0x9e, 0x5e, 0x7b, 0x2, 0x8d, 0xa6, 0xc7 } };  // {870DDBD6-DAF1-4877-A89E-5E7B028DA6C7}
	static constexpr GUID signal_Steil_Rebrin_Blood = { 0x287b9bb8, 0xb73b, 0x4485,{ 0xbe, 0x20, 0x2c, 0x8c, 0x40, 0x98, 0x3b, 0x16 } }; // {287B9BB8-B73B-4485-BE20-2C8C40983B16}

	//virtual signals used for temporal remapping
	extern const std::array<GUID, 100> signal_Virtual;

	using IModel_Parameter_Vector = refcnt::double_container;

	enum class NDevice_Event_Code : uint8_t {
		Nothing = 0,		//internal event of the object
		Shut_Down,

		Level,				//level contains newly measured or calculated level of a given signal
		Masked_Level,		//level not advertised for solving, i.e., tranining, but for testing set
		Parameters,			//new parameters are available for a given signal
		Parameters_Hint,	//some solver requires e.g., initial estimate of the model parameters so that
							//params stored in non-volatile memory should be broadcasted so that
							//solvers and calculators can use them

		//-------- simulation related codes ------
		Suspend_Parameter_Solving,
		Resume_Parameter_Solving,
		Solve_Parameters,	//user can either request to recalculate, or we can request to recalculate it at the end of the segment - i.e., prior sending Time_Segment_Stop
		Time_Segment_Start,
		Time_Segment_Stop,
		Warm_Reset,			//all incoming levels (and associated errors) are thrown away, calculated parameters are kept and DB/File input filters replays the data from begining, while CGMS input filter just ignores this message
		Simulation_Step,


		//-------- codes intended for log parsers ------
		Information,
		Warning,
		Error,

		count
	};


	struct TDevice_Event {
		NDevice_Event_Code event_code;

		GUID device_id;					//supporting parallel measurements
		GUID signal_id;					//blood, ist, isig, model id aka e.g, calculated blood, etc.

		double device_time;				//signal with multiple values are aggregated byt device_time with the same signal_id and device_id
		int64_t logical_time;

		uint64_t segment_id;			// segment identifier or Invalid_Segment_Id

		union {
			double level;
			IModel_Parameter_Vector* parameters;		//this will have to be marshalled
														//as different models have different number of parameters, statically sized field would case over-complicated code later on
			refcnt::wstr_container* info;				//information, warning, error 
		};
	};

	//To make TDevice_Event handling more efficient, particulalry when passing through the pipe,
	//IDevice_Event exposes TDevice_Event container iface so that the pipe can accept and pass throught only a pointer, not the entire structure.
	//This way, we avoid the overhead of copying size_of(TDevice_Event) so many times.
	class IDevice_Event : public virtual refcnt::IUnique_Reference {
	public:
		// provides pointer to the contained TDevice_Event (free to modify as needed)
		virtual HRESULT IfaceCalling Raw(TDevice_Event **raw) = 0;
	};

	static constexpr decltype(TDevice_Event::segment_id) Invalid_Segment_Id = std::numeric_limits<decltype(Invalid_Segment_Id)>::max();
	static constexpr decltype(TDevice_Event::segment_id) All_Segments_Id = std::numeric_limits<decltype(All_Segments_Id)>::max() - 1;

	typedef struct {
		double Min, Max;
	} TBounds;


	class ISignal : public virtual refcnt::IReferenced {
	public:
		/* on S_OK, *filled elements were copied into times and double levels of the count size
		   for measured signal, it returns the measured values
		   for calculated signal, it returns E_NOIMPL
		*/
		virtual HRESULT IfaceCalling Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const = 0;

		/* gets bounds and level_count, any of these parameters can be nullptr
		   for measured and calculated signals, dtto Get_Discrete_Levels
		*/
		virtual HRESULT IfaceCalling Get_Discrete_Bounds(TBounds* const time_bounds, TBounds* const level_bounds, size_t *level_count) const = 0;

		/* adds measured levels to internal containers
		   for measured and calculated signals, dtto Get_Discrete_Levels
		*/
		virtual HRESULT IfaceCalling Add_Levels(const double *times, const double *levels, const size_t count) = 0;

		/*
			this method will be called in parallel by solvers and therefore it has to be const

			params - params from which to calculate the signal
						can be nullptr to indicate use of default parameters
			times - times at which to get the levels, i.e., y values for x values
			levels - the levels, must be already allocated with size of count
					- level that cannot be calculated must be se to quiet nan
			count - the total number of times for which to get the levels
			derivation_order - order of derivation requested
		*/
		virtual HRESULT IfaceCalling Get_Continuous_Levels(IModel_Parameter_Vector *params,
			const double* times, double* const levels, const size_t count, const size_t derivation_order) const = 0;

		// returns default parameters on calculated signals, E_NOIMPL on measured signal
		virtual HRESULT IfaceCalling Get_Default_Parameters(IModel_Parameter_Vector *parameters) const = 0;
	};


	class ITime_Segment : public virtual refcnt::IReferenced {
	public:
		// retrieves or creates signal with given id; calls AddRef on returned object
		virtual HRESULT IfaceCalling Get_Signal(const GUID *signal_id, ISignal **signal) = 0;
	};

	// segment provides source levels for the calculation
	// only ITime_Segment::Get_Signal is supposed to call this function to avoid (although not probihit) creating of over-complex segment-graphs
	using TCreate_Signal = HRESULT(IfaceCalling *)(const GUID *calc_id, ITime_Segment *segment, ISignal **signal);

	using TCreate_Device_Event = HRESULT(IfaceCalling *)(glucose::NDevice_Event_Code code, glucose::IDevice_Event **event);
}
