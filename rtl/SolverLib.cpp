#include "SolverLib.h"

namespace imported {
	//#define DIMPORT_TEST_FAIL E_NOTIMPL

	#ifdef DIMPORT_TEST_FAIL
		HRESULT IfaceCalling create_metric(const glucose::TMetric_Parameters *parameters, glucose::IMetric **metric) {
			return DIMPORT_TEST_FAIL;
		}

		HRESULT IfaceCalling solve_model_parameters(const glucose::TSolver_Setup *setup) {
			return DIMPORT_TEST_FAIL;
		}

	#else
		#ifdef _WIN32
			extern "C" __declspec(dllimport) HRESULT IfaceCalling solve_model_parameters(const glucose::TSolver_Setup *setup);
			extern "C" __declspec(dllimport) HRESULT IfaceCalling create_metric(const glucose::TMetric_Parameters *parameters, glucose::IMetric **metric);
		#endif
	#endif
}

glucose::SMetric::SMetric() : std::shared_ptr<glucose::IMetric>() {
	glucose::TMetric_Parameters params{ 0 };
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
	glucose::TMetric_Parameters params{0};

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