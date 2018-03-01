#pragma once

#include "../iface/DeviceIface.h"

#include <memory>
#include <vector>

namespace glucose {

	using SModel_Parameter_Vector = std::shared_ptr<IModel_Parameter_Vector>;

	using SSignal = std::shared_ptr<ISignal>;

	class WTime_Segment { //: public std::weak_ptr<ITime_Segment> { --cannot inherit because shared-weak relationshop cannot live across COM interface
						  //Perhaps, we should make STime_Segment export friendly function to WTimeSegment that will take WTime_Segment's
						  //as callback of STime_Segment. STime_Segment will call the WTime_Segment's callback from STime_Segment's dctor.
						  //That way, WTime_Segment will learn about STime_Segment's desctruction and return appropriate error codes from that moment on.
	protected:
		ITime_Segment * mSegment;
	public:
		WTime_Segment(ITime_Segment *segment);
		SSignal Get_Signal(const GUID &signal_id);
	};

	class STime_Segment : public std::shared_ptr<ITime_Segment> {
	public:
		SSignal Get_Signal(const GUID &signal_id);
	};

}