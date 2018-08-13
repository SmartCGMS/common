#include "SolverLib.h"
#include "DeviceLib.h"

#include "FactoryLib.h"

namespace imported {
	glucose::TSolve_Model_Parameters solve_model_parameters = factory::resolve_symbol<glucose::TSolve_Model_Parameters>("solve_model_parameters");
	glucose::TCreate_Metric create_metric = factory::resolve_symbol<glucose::TCreate_Metric>("create_metric");
}

glucose::SMetric::SMetric() : std::shared_ptr<glucose::IMetric>() {
	glucose::TMetric_Parameters params = glucose::Null_Metric_Parameters;
	Init(params);
}

glucose::SMetric::SMetric(const glucose::TMetric_Parameters &params) : std::shared_ptr<glucose::IMetric>() {
	Init(params);
}

void glucose::SMetric::Init(const glucose::TMetric_Parameters &params)
{
	glucose::IMetric* metric;
	if (imported::create_metric(&params, &metric) == S_OK) {
		reset(metric, [](glucose::IMetric* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}
}

glucose::SMetric glucose::SMetric::Clone()
{
	glucose::SMetric result;
	auto self = get();
	glucose::TMetric_Parameters params = glucose::Null_Metric_Parameters;

	if (self && self->Get_Parameters(&params) == S_OK)
	{
		glucose::IMetric *obj = nullptr;
		if (imported::create_metric(&params, &obj) == S_OK)
			result = refcnt::make_shared_reference_ext<glucose::SMetric, glucose::IMetric>(obj, false);
	}

	return result;
}


HRESULT glucose::Solve_Model_Parameters(const TSolver_Setup &setup) {
	return imported::solve_model_parameters(&setup);
}