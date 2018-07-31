#include "ApproxLib.h"

#include "FactoryLib.h"

namespace imported {
	glucose::TCreate_Approximator create_approximator = factory::resolve_symbol<glucose::TCreate_Approximator>("create_approximator");
	glucose::TGet_Approx_Descriptors get_approx_descriptors = factory::resolve_symbol<glucose::TGet_Approx_Descriptors>("get_approx_descriptors");
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