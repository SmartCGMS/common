/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

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