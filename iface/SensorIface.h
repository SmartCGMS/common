#pragma once

#include "SignalIface.h"

namespace glucose {


	enum class NSensor_Event_Code : uint_least16_t
	{
		Nothing = 0,

		Measured_IG = 1,		//level and isig must be valid
		Measured_BG = 2,
		Calibration = 3,		//event_level contains reference BG
		Insulin_Dose = 4,
		Carb_Intake = 5,
		Health_Stress = 6,
	};


	typedef struct {
		size_t logical_clock;
		floattype level;
		floattype isig;
		floattype date_time;
		NSensor_Event_Code event_code;
		floattype event_level;
	} TSensor_Record;

	class ISensor : public virtual IReference {
		virtual HRESULT IfaceCalling Get_Records(const size_t starting_clock, const size_t clock_count, TSensor_Record *dst, size_t *filled) = 0;
	};

}