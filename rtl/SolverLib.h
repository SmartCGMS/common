#pragma once

#include "../iface/SolverIface.h"

#include <memory>

namespace glucose {
	
	class SMetric : public std::shared_ptr<IMetric> {
	public:
		SMetric Clone();
	};


}