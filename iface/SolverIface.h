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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

#include "FilterIface.h"

#undef min

namespace solver {
	constexpr size_t Maximum_Objectives_Count = 10;

	//solver sets these values to indicate its progress
	struct TSolver_Progress {
		size_t current_progress, max_progress;	//minimum progress is zero
		double best_metric;
		BOOL cancelled;	//just cast it to bool, if set to true, solver cancels the current operation
	};

	const TSolver_Progress Null_Solver_Progress = { 0, 0, 0.0, 0 };
	
	using TObjective_Function = BOOL(IfaceCalling*)(const void *data, const double *solution, double * const fitness);
		//data is an opaque handler
		//solution points to the current candidate solution
		//fitness is an array where to store up to Maximum_Objective_Count
		//returns TRUE or FALSE

	struct TSolver_Setup {
		const size_t problem_size;
		const size_t objectives_count;			//1 for single objective, else the number of objectives
		const double *lower_bound, *upper_bound;
		const double **hints;
		const size_t hint_count;
		double * const solution;

		const void *data;
		const TObjective_Function objective;

		const size_t max_generations;	//where relevant, maximum number of generations - zero for default value
		const size_t population_size;	//where relevant, maximum number of population - zero for default value
		const double tolerance;			//where relevant, objective function tolerance that indicates no further improvement
	};


	const TSolver_Setup Default_Solver_Setup = { 0, 0, nullptr, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, std::numeric_limits<double>::min() };
	using TGeneric_Solver = HRESULT(IfaceCalling*)(const GUID *solver_id, const TSolver_Setup *setup, TSolver_Progress *progress);
}

namespace scgms {

	struct TMetric_Parameters {
		const GUID metric_id;
			//any metric can ignore the parameters below as seen fit as e.g., AIC or Leal_2010 are not compatible with all the options
		const unsigned char use_relative_error;
		const unsigned char use_squared_differences;
		const unsigned char prefer_more_levels;		//i.e. result is once more divided by the number of levels evaluated
		const double threshold;					//particular meaning depends on used metric and the caller is responsible for providing correct value
	};

	const TMetric_Parameters Null_Metric_Parameters = { Invalid_GUID, 0, 0, 0, 0.0 };

	class IMetric: public virtual refcnt::IReferenced {
	public:
		/* let's the calculator to process a batch of known differences
		   count is the number of elements of differences, which are encoded as vectors to exploit SIMD
				this will become significant with ist prediction, where increased number of levels is expected compared to blood
		   count is the total number of all levels that could have been calculated under optimal conditions
				not calculated levels are quiet NaN and they are ignored
		*/
		virtual HRESULT IfaceCalling Accumulate(const double *times, const double *reference, const double *calculated, const size_t count) = 0;

		// undo all previously called Accumulate
		virtual HRESULT IfaceCalling Reset() = 0;

		/* calculates the metric - the less number is better
		   levels_accumulated will be the number of non-nan levels accumulated, can be nullptr
		   returns S_FALSE if *levels_accumulated < levels_required
		*/
		virtual HRESULT IfaceCalling Calculate(double *metric, size_t *levels_accumulated, size_t levels_required) = 0;

		// Retrieves metric parameter struct, so that we can clone the metric, e.g., over a network connection
		virtual HRESULT IfaceCalling Get_Parameters(TMetric_Parameters *parameters) = 0;
	};

	enum class TSolver_Status : uint8_t {
		Disabled = 0,
		Idle,
		In_Progress,
		Completed_Improved,
		Completed_Not_Improved,
		Failed
	};

	
	constexpr GUID IID_Calculate_Filter_Inspection = { 0xec44cd18, 0x8d08, 0x46d1, { 0xa6, 0xcb, 0xc2, 0x43, 0x8e, 0x4, 0x19, 0x88 } };
	class ICalculate_Filter_Inspection : public virtual refcnt::IReferenced {
	public:
		// makes a deep copy of the entire progress
		virtual HRESULT IfaceCalling Get_Solver_Progress(solver::TSolver_Progress* const progress) = 0;
		// retrieves solver information
		virtual HRESULT IfaceCalling Get_Solver_Information(GUID* const calculated_signal_id, scgms::TSolver_Status* const status) const = 0;
		// explicitly cancels solver
		virtual HRESULT IfaceCalling Cancel_Solver() = 0;
	};

	using TCreate_Metric = HRESULT(IfaceCalling*)(const TMetric_Parameters *parameters, IMetric **metric);

	using TOptimize_Parameters = HRESULT(IfaceCalling*)(scgms::IFilter_Chain_Configuration *configuration, const size_t filter_index, const wchar_t *parameters_configuration_name,
		scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data,
		const GUID *solver_id, const size_t population_size, const size_t max_generations, 
		const double** hints, const size_t hint_count,
		solver::TSolver_Progress *progress, refcnt::wstr_list *error_description);

	using TOptimize_Multiple_Parameters = HRESULT(IfaceCalling*)(scgms::IFilter_Chain_Configuration *configuration, const size_t *filter_indices, const wchar_t **parameters_configuration_names, size_t filter_count,
		scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data,
		const GUID *solver_id, const size_t population_size, const size_t max_generations, 
		const double** hints, const size_t hint_count,
		solver::TSolver_Progress *progress, refcnt::wstr_list *error_description);
}
