#pragma once

#include "../iface/SolverIface.h"

#include <memory>

namespace glucose {
	
	class SMetric : public std::shared_ptr<IMetric> {
	private:
		void Init(const glucose::TMetric_Parameters &params);
	public:
		SMetric();
		SMetric(const TMetric_Parameters &params);
		SMetric Clone();
	};

	HRESULT Solve_Model_Parameters(const TSolver_Setup &setup);

}