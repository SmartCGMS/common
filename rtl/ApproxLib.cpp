#include "ApproxLib.h"

namespace imported {
	//#define DIMPORT_TEST_FAIL E_NOTIMPL

	#ifdef DIMPORT_TEST_FAIL
		HRESULT IfaceCalling create_approximator(const GUID *approx_id, glucose::ISignal *signal, glucose::IApprox_Parameters_Vector* configuration, glucose::IApproximator **approx) {
			return DIMPORT_TEST_FAIL;
		}

		HRESULT IfaceCalling get_approx_descriptors(glucose::TApprox_Descriptor **begin, glucose::TApprox_Descriptor **end) {
			return DIMPORT_TEST_FAIL;
		}
	#else
		#ifdef _WIN32
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_approximator(const GUID *approx_id, glucose::ISignal *signal, glucose::IApprox_Parameters_Vector* configuration, glucose::IApproximator **approx);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_approx_descriptors(glucose::TApprox_Descriptor **begin, glucose::TApprox_Descriptor **end);
		#else
			extern "C" HRESULT IfaceCalling create_approximator(const GUID *approx_id, glucose::ISignal *signal, glucose::IApprox_Parameters_Vector* configuration, glucose::IApproximator **approx);
			extern "C" HRESULT IfaceCalling get_approx_descriptors(glucose::TApprox_Descriptor **begin, glucose::TApprox_Descriptor **end);
		#endif
	#endif
}

std::vector<glucose::TApprox_Descriptor> glucose::get_approx_descriptors() {
	std::vector<glucose::TApprox_Descriptor> result;
	glucose::TApprox_Descriptor *desc_begin, *desc_end;

	if (imported::get_approx_descriptors(&desc_begin, &desc_end) == S_OK) 
		std::copy(desc_begin, desc_end, std::back_inserter(result));
	

	return result;
}



glucose::SApproximator glucose::Create_Approximator(const GUID &id, glucose::SSignal signal, glucose::SApprox_Parameters_Vector configuration) {
	return Create_Approximator(id, signal.get(), configuration);
}

glucose::SApproximator glucose::Create_Approximator(const GUID &id, glucose::ISignal* signal, glucose::SApprox_Parameters_Vector configuration) {

	glucose::SApproximator result;
	glucose::IApproximator *approximator;

	if (imported::create_approximator(&id,  signal, configuration.get(), &approximator) == S_OK)
		result = refcnt::make_shared_reference_ext<glucose::SApproximator, glucose::IApproximator>(approximator, false);

	return result;
}