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

#pragma once

#include "../iface/SolverIface.h"
#include "../rtl/FilterLib.h"

#include <memory>

namespace solver {
	solver::TSolver_Setup Check_Default_Parameters(const solver::TSolver_Setup &setup, const size_t default_max_generations, const size_t default_population_size);	
}

namespace scgms {
	template <typename T>
	T& Convert_Parameters(scgms::IModel_Parameter_Vector *params, const double *default_parameters) {
		double *begin{ const_cast<double*>(default_parameters) };	//just in case that no parameters are set at all -> than we have to use the default ones
		if (params) {
			double *tmp_begin, *end;
			if (params->get(&tmp_begin, &end) == S_OK) {
				//not that params still could be empty
				if (tmp_begin && (tmp_begin != end))  begin = tmp_begin;
			}
		}

		T &result = *(reinterpret_cast<T*>(begin));
		return result;
	}

	
	class SMetric : public std::shared_ptr<IMetric> {
	private:
		void Init(const scgms::TMetric_Parameters &params);
	public:
		SMetric();
		SMetric(const TMetric_Parameters &params);
		SMetric Clone();
	};

	class SCalculate_Filter_Inspection : public std::shared_ptr<ICalculate_Filter_Inspection> {
	public:
		SCalculate_Filter_Inspection() noexcept {};
		SCalculate_Filter_Inspection(SFilter &calculate_filter);
	};


	HRESULT Solve_Model_Parameters(const scgms::TSolver_Setup &setup);

	HRESULT Optimize_Parameters(scgms::SFilter_Chain_Configuration configuration, const size_t filter_index, const wchar_t *parameters_configuration_name,								
								scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data,
								const GUID &solver_id, const size_t population_size, const size_t max_generations, solver::TSolver_Progress &progress);

}