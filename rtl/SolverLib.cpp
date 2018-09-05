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