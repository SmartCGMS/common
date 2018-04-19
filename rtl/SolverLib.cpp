#include "SolverLib.h"

namespace imported {
	#ifdef _WIN32
		extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_metric(const glucose::TMetric_Parameters *parameters, glucose::IMetric **metric);
	#endif
}

glucose::SMetric(const TMetric_Parameters &params) : std::shared_ptr<IMetric>() {
	glucose::IMetric metric;
	if (create_metric(&params, &metric) == S_OK) {
		reset(metric);
	}
}

glucose::SMetric glucose::SMetric::Clone() {
	glucose::SMetric result;
	auto self = get();
	glucose::TMetric_Parameters params{0};

	if (self)
		if (self->Get_Parameters(&params) == S_OK) {
			glucose::IMetric *obj = nullptr;
			if (imported::create_metric(&params, &obj) == S_OK)
				result = refcnt::make_shared_reference_ext<glucose::SMetric, glucose::IMetric>(obj, false);
		}

	return result;
}