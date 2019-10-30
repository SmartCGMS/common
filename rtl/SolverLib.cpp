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

#include "SolverLib.h"
#include "DeviceLib.h"

#include "FactoryLib.h"

namespace imported {
	glucose::TSolve_Model_Parameters solve_model_parameters = factory::resolve_symbol<glucose::TSolve_Model_Parameters>("solve_model_parameters");
	glucose::TCreate_Metric create_metric = factory::resolve_symbol<glucose::TCreate_Metric>("create_metric");
	glucose::TOptimize_Parameters = optimize_parameters = factory::resolve_symbol<glucose::TOptimize_Parameters>("optimize_parameters");
}

solver::TSolver_Setup solver::Check_Default_Parameters(const solver::TSolver_Setup &setup, const size_t default_max_generations, const size_t default_population_size) {
	//fill in the default values
	solver::TSolver_Setup result{
			setup.problem_size,
			setup.lower_bound, setup.upper_bound,
			setup.hints, setup.hint_count,
			setup.solution,

			setup.data, setup.objective,

			setup.max_generations == 0 ? default_max_generations : setup.max_generations,
			setup.population_size == 0 ? default_population_size : setup.population_size,
			setup.tolerance
	};

	return result;
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

HRESULT glucose::Solve_Model_Parameters(const glucose::TSolver_Setup &setup) {
	return imported::solve_model_parameters(&setup);
}
