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

}