#pragma once

#include "ModelIface.h"
#include "LogicalClockIface.h"

namespace glucose {

	typedef double floattype;

	//Units used for glucose levels 
	enum class TUnits
	{
		MmolPerL, //mmol/l
		MgPerDl,	//mg/dl
		DgPerDl,	//dg/dl
	};

	//constant to convert mg/dl to mmol/l
	extern const floattype mgdl2mmoll;	//we assume mmol/l, so to make a conversion possible

	extern const floattype One_Hour;
	extern const floattype One_Minute;
	extern const floattype One_Second;

	typedef struct {
		floattype date_time;	//time of measuring
		floattype level;		//the glucose level/concentration measured
	} TLevel;

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

	typedef struct {
		floattype Min_Time,
				  Max_Time,
				  Min_Level,
				  Max_Level;
	} TBounds;

	
	class IDiscrete_Signal : public virtual IReferenced, public virtual ILogical_Clock {
	public:
		virtual HRESULT IfaceCalling Get_Levels(const TLevel *dst, const size_t dst_size, size_t *filled) = 0;
			//on S_OK, *filled TLevel elements were copied into the dst buffer of dst_size size

		virtual HRESULT IfaceCalling Get_Bounds(TBounds *bounds, size_t *level_count) = 0;
			//gets bounds and level_count, any of these parameters can be nullptr

		virtual HRESULT IfaceCalling Add_Levels(const TLevel *src, const size_t src_size) = 0;	
			//incerements ILogical_Clock::Clock
	};

	class IContinuous_Signal : public virtual IReferenced {
	public:
		virtual HRESULT IfaceCalling Get_Levels(const TModel_Params *params,
												const floattype *times, const floattype *levels, const size_t count,
												size_t *filled, const size_t derivation_order) = 0 const;
		/* 
		this method will be called in parallel by solvers and therefore it has to be const

		params - params from which to calculate the signal
		times - times at which to get the levels, i.e., y values for x values		
		count - the total number of times for which to get the levels
		levels - the levels, must be already allocated with size of count
		filled - the number of levels filled
		*/

		virtual HRESULT IfaceCalling Get_Control_Levels(IDiscrete_Signal **control_levels) = 0;
			//Gets levels, which drive shape of the continuous signal - e.g.; measured levels, which we approximate
	};

}