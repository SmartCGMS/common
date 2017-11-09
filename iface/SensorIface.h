#pragma once

namespace glucose {


	enum class NSensor_Event_Code {
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
		double level;
		double isig;
		double date_time;		
		double event_level;
	};

	
}