/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#include "ApproxLib.h"

#include "scgmsLib.h"

namespace imported {
	namespace {
		scgms::TCreate_Approximator create_approximator_external = scgms::factory::resolve_symbol<scgms::TCreate_Approximator>("create_approximator");
		scgms::TGet_Approx_Descriptors get_approx_descriptors_external = scgms::factory::resolve_symbol<scgms::TGet_Approx_Descriptors>("get_approx_descriptors");
	}
}

std::vector<scgms::TApprox_Descriptor> scgms::get_approx_descriptor_list() {
	std::vector<scgms::TApprox_Descriptor> result;
	scgms::TApprox_Descriptor *desc_begin, *desc_end;

	if (imported::get_approx_descriptors_external(&desc_begin, &desc_end) == S_OK)
		std::copy(desc_begin, desc_end, std::back_inserter(result));

	return result;
}


scgms::SApproximator Create_Approximator_Raw_ID(const GUID* id, scgms::ISignal* signal) {

	scgms::SApproximator result;
	scgms::IApproximator* approximator;

	if (imported::create_approximator_external(id, signal, &approximator) == S_OK)
		result = refcnt::make_shared_reference_ext<scgms::SApproximator, scgms::IApproximator>(approximator, false);

	return result;
}



scgms::SApproximator scgms::Create_Approximator(const GUID &id, scgms::SSignal signal) {
	return Create_Approximator_Raw_ID(&id, signal.get());
}


scgms::SApproximator scgms::Create_Approximator(const GUID &id, scgms::ISignal* signal) {
	return Create_Approximator_Raw_ID(&id, signal);	
}

scgms::SApproximator scgms::Create_Approximator(scgms::ISignal* signal) {
	return Create_Approximator_Raw_ID(nullptr, signal);
}