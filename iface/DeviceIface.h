#pragma once

#include "../rtl/guid.h"
#include "referencedIface.h"

namespace glucose {

	//constant to convert mg/dl to mmol/l
	extern const double mgdl_2_mmoll;	//we assume mmol/l, so to make a conversion possible

	extern const double One_Hour;
	extern const double One_Minute;
	extern const double One_Second;

	struct TLevel {
		double date_time;	//time of measuring
		double level;		//the glucose level/concentration measured
	};

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

	static constexpr GUID signal_BG = { 0xf666f6c2, 0xd7c0, 0x43e8,{ 0x8e, 0xe1, 0xc8, 0xca, 0xa8, 0xf8, 0x60, 0xe5 } };	// {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}
	static constexpr GUID signal_IG = { 0x3034568d, 0xf498, 0x455b,{ 0xac, 0x6a, 0xbc, 0xf3, 0x1, 0xf6, 0x9c, 0x9e } };		// {3034568D-F498-455B-AC6A-BCF301F69C9E}
	static constexpr GUID signal_ISIG =	{ 0x3f62c28a, 0x4d25, 0x4086,{ 0xbd, 0x1a, 0xfc, 0x44, 0x2f, 0xdd, 0xb7, 0xcf } };		// {3F62C28A-4D25-4086-BD1A-FC442FDDB7CF}
	static constexpr GUID signal_Insulin = { 0x22d87566, 0xaf1b, 0x4cc7,{ 0x8d, 0x11, 0xc5, 0xe0, 0x4e, 0x1e, 0x9c, 0x8a } }; 	 // {22D87566-AF1B-4CC7-8D11-C5E04E1E9C8A}
	static constexpr GUID signal_Carb_Intake = { 0x37aa6ac1, 0x6984, 0x4a06,{ 0x92, 0xcc, 0xa6, 0x60, 0x11, 0xd, 0xd, 0xc7 } };	// {37AA6AC1-6984-4A06-92CC-A660110D0DC7}																																		
	static constexpr GUID signal_Health_Stress = { 0xf4438e9a, 0xdd52, 0x45bd,{ 0x83, 0xce, 0x5e, 0x93, 0x61, 0x5e, 0x62, 0xbd } }; // {F4438E9A-DD52-45BD-83CE-5E93615E62BD}


	using IModel_Parameter_Vector = refcnt::double_container;

	enum class NDevice_Event_Code : uint8_t {
		Nothing = 0,		//internal event of the object

		Level,				//level contains newly measured or calculated level of a given signal
		Calibrated,			//given device was calibrated using level
		Parameters,			//new parameters are available for a given signal

		//-------- simulation related codes ------
		Time_Segment_Start,
		Time_Segment_Stop,


		//-------- codes intended for log parsers ------
		Information,
		Warning,
		Error
	};


	struct TDevice_Event {		
		NDevice_Event_Code event_code;

		GUID device_id;					//supporting parallel measurements
		GUID signal_id;					//blood, ist, isig, model id aka e.g, calculated blood, etc.

		double device_time;				//signal with multiple values are aggregated byt device_time with the same signal_id and device_id
		int64_t logical_time;			

		union {
			double level;
			IModel_Parameter_Vector* parameters;		//this will have to be marshalled
														//as different models have different number of parameters, statically sized field would case over-complicated code later on
			refcnt::wstr_container* info;				//information, warning, error 
		};
	};


	typedef struct {
		double Min_Time,
			Max_Time,
			Min_Level,
			Max_Level;
	} TBounds;


	class ISignal : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Get_Discrete_Levels(const TLevel *dst, const size_t dst_size, size_t *filled) const = 0;
		//on S_OK, *filled TLevel elements were copied into the dst buffer of dst_size size

		virtual HRESULT IfaceCalling Get_Discrete_Bounds(TBounds *bounds, size_t *level_count) const = 0 ;
		//gets bounds and level_count, any of these parameters can be nullptr

		virtual HRESULT IfaceCalling Add_Levels(const TLevel *begin, const TLevel *end) = 0;

		virtual HRESULT IfaceCalling Get_Continuous_Levels(IModel_Parameter_Vector *params,
			const double *times, const double *levels, const size_t count,
			size_t *filled, const size_t derivation_order) const = 0;
			/*
				this method will be called in parallel by solvers and therefore it has to be const

				params - params from which to calculate the signal
							can be nullptr to indicate use of default parameters
				times - times at which to get the levels, i.e., y values for x values
				count - the total number of times for which to get the levels
				levels - the levels, must be already allocated with size of count
				filled - the number of levels filled
			*/		
	};


	class ITime_Segment : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Get_Signal(const GUID *signal_id, ISignal **signal) = 0;
			//calls AddRef on returned object
		virtual HRESULT IfaceCalling Suggest_Parameters(const GUID *signal_id, IModel_Parameter_Vector **suggestions, const size_t suggestions_count, size_t *suggested) = 0;
			//returns e.g., parameters which were calculated previously and stored in a non-volatile memory
			//if there are no such parameters, the segment should attempt to get and return default parameters from the model description that it will lookup via the signal id			
			//signal_id is to be possibly used by the segment to evalate fitness per signal if model provides more than one
	};

	using TCreate_Calculated_Signal = HRESULT(IfaceCalling *)(const GUID *calc_id, ITime_Segment *segment, ISignal **signal);
		//segment provides source levels for the calculation
		//only ITime_Segment::Get_Signal is supposed to call this function to avoid (although not probihit) creating of over-complex segment-graphs
	
}