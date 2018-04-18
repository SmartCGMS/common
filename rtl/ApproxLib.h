#pragma once

#include "../iface/UIIface.h"
#include "../iface/ApproxIface.h"
#include "../rtl/DeviceLib.h"

#include "referencedImpl.h"
#include <vector>

namespace glucose {
	std::vector<TApprox_Descriptor> get_approx_descriptors();

	using SApproximator = std::shared_ptr<IApproximator>;
	using SApprox_Parameters_Vector = std::shared_ptr<IApprox_Parameters_Vector>;
	SApproximator Create_Approximator(const GUID &id, SSignal signal, SApprox_Parameters_Vector configuration);
}