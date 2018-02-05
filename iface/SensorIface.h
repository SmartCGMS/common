#pragma once

namespace glucose {


	enum class NSensor_Event_Code : uint8_t {
		Nothing,			//internal event of the object

		Measured_IG,		//level and isig must be valid
		Measured_BG,
		Calibration,		//event_level contains reference BG
		Insulin_Dose,
		Carb_Intake,
		Health_Stress,
	};


	struct TSensor_Event {		
		NSensor_Event_Code event_code;
		double device_time;
		int64_t logical_time;

		double level;
		double isig;
		double event_level;		
	};

	
}