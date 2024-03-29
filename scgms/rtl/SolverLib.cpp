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

#include "SolverLib.h"
#include "DeviceLib.h"

#include "scgmsLib.h"

namespace imported {
	namespace {
		//all these vars have _external suffix not to confuse linker when building the libraries
		scgms::TCreate_Metric create_metric_external = scgms::factory::resolve_symbol<scgms::TCreate_Metric>("create_metric");
		scgms::TOptimize_Parameters optimize_parameters_external = scgms::factory::resolve_symbol<scgms::TOptimize_Parameters>("optimize_parameters");
		scgms::TOptimize_Multiple_Parameters optimize_multiple_parameters_external = scgms::factory::resolve_symbol<scgms::TOptimize_Multiple_Parameters>("optimize_multiple_parameters");
		solver::TGeneric_Solver generic_solver_external = scgms::factory::resolve_symbol<solver::TGeneric_Solver>("solve_generic");
	}
}

solver::TSolver_Setup solver::Check_Default_Parameters(const solver::TSolver_Setup &setup, const size_t default_max_generations, const size_t default_population_size) {
	//fill in the default values
	solver::TSolver_Setup result{
			setup.problem_size,
			std::min(setup.objectives_count, solver::Maximum_Objectives_Count),
			setup.lower_bound, setup.upper_bound,
			setup.hints, setup.hint_count,
			setup.solution,

			setup.data, setup.objective, setup.comparator,

			setup.max_generations == 0 ? default_max_generations : setup.max_generations,
			setup.population_size == 0 ? default_population_size : setup.population_size,
			setup.tolerance
	};

	return result;
}


HRESULT solver::Solve_Generic(const GUID &solver_id, const solver::TSolver_Setup &setup, solver::TSolver_Progress &progress) noexcept {
	return imported::generic_solver_external(&solver_id, &setup, &progress);
}

scgms::SMetric::SMetric() : std::shared_ptr<scgms::IMetric>() {
	scgms::TMetric_Parameters params = scgms::Null_Metric_Parameters;
	Init(params);
}

scgms::SMetric::SMetric(const scgms::TMetric_Parameters &params) : std::shared_ptr<scgms::IMetric>() {
	Init(params);
}

void scgms::SMetric::Init(const scgms::TMetric_Parameters &params)
{
	scgms::IMetric* metric;
	if (imported::create_metric_external(&params, &metric) == S_OK) {
		reset(metric, [](scgms::IMetric* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}
}

scgms::SMetric scgms::SMetric::Clone()
{
	scgms::SMetric result;
	auto self = get();
	scgms::TMetric_Parameters params = scgms::Null_Metric_Parameters;

	if (self && self->Get_Parameters(&params) == S_OK)
	{
		scgms::IMetric *obj = nullptr;
		if (imported::create_metric_external(&params, &obj) == S_OK)
			result = refcnt::make_shared_reference_ext<scgms::SMetric, scgms::IMetric>(obj, false);
	}

	return result;
}

scgms::SCalculate_Filter_Inspection::SCalculate_Filter_Inspection(const scgms::SFilter &calculate_filter) {
	if (calculate_filter)
		refcnt::Query_Interface<scgms::IFilter, scgms::ICalculate_Filter_Inspection>(calculate_filter.get(), IID_Calculate_Filter_Inspection, *this);
}


HRESULT scgms::Optimize_Parameters(scgms::SFilter_Chain_Configuration configuration, const size_t *filter_indices, const wchar_t **parameters_configuration_names, size_t filter_count,
	scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data,
	const GUID &solver_id, const size_t population_size, const size_t max_generations, 
	const double** hints, const size_t hint_count, 
	solver::TSolver_Progress &progress,
	refcnt::Swstr_list error_description) {

	return imported::optimize_multiple_parameters_external(configuration.get(), filter_indices, parameters_configuration_names, filter_count,
		on_filter_created, on_filter_created_data, &solver_id, population_size, max_generations, 
		hints, hint_count,
		&progress, error_description.get());
}
