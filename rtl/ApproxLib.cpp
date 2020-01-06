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
 * Univerzitni 8
 * 301 00, Pilsen
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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "ApproxLib.h"

#include "FactoryLib.h"

namespace imported {
	scgms::TCreate_Approximator create_approximator = factory::resolve_symbol<scgms::TCreate_Approximator>("create_approximator");
	scgms::TGet_Approx_Descriptors get_approx_descriptors = factory::resolve_symbol<scgms::TGet_Approx_Descriptors>("get_approx_descriptors");
}

std::vector<scgms::TApprox_Descriptor> scgms::get_approx_descriptors() {
	std::vector<scgms::TApprox_Descriptor> result;
	scgms::TApprox_Descriptor *desc_begin, *desc_end;

	if (imported::get_approx_descriptors(&desc_begin, &desc_end) == S_OK) 
		std::copy(desc_begin, desc_end, std::back_inserter(result));

	return result;
}



scgms::SApproximator scgms::Create_Approximator(const GUID &id, scgms::SSignal signal, scgms::SApprox_Parameters_Vector configuration) {
	return Create_Approximator(id, signal.get(), configuration);
}

scgms::SApproximator scgms::Create_Approximator(const GUID &id, scgms::ISignal* signal, scgms::SApprox_Parameters_Vector configuration) {

	scgms::SApproximator result;
	scgms::IApproximator *approximator;

	if (imported::create_approximator(&id,  signal, configuration.get(), &approximator) == S_OK)
		result = refcnt::make_shared_reference_ext<scgms::SApproximator, scgms::IApproximator>(approximator, false);

	return result;
}
