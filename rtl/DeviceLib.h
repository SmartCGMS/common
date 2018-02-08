#pragma once

#include "../iface/DeviceIface.h"

#include <memory>
#include <vector>

namespace glucose {

	using SModel_Parameter_Vector = std::shared_ptr<IModel_Parameter_Vector>;

	using SSignal = std::shared_ptr<ISignal>;

	class STime_Segment :public std::shared_ptr<ITime_Segment> {
	public:
		SSignal Get_Signal(const GUID &signal_id);
	};


	std::vector<STime_Segment> Time_Segments_To_Vector(const ITime_Segment **segments, const size_t segment_count);

}