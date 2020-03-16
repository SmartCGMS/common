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

#include "DeviceIface.h"
#include "DbIface.h"

 //TODO: create rtl/opencllib.h with at least a function to rename OpenCL function to avoid name conflicts

namespace opencl {		//note that opencl does not clash with the official cl namespace

	enum class NFunction_Id : uint16_t {
		Metric_Calculate_Accumulated = 0,		//equivalent to calling scgms::IMetric->Reset, Accumulate and Calculate
												//parameters[0].integer gives the number of elements in calculated and expected arrays respectively
												//parameters[1].integer gives the number of required levels
												//returns a function source with the following signature
												//double Metric_Calculate_Accumulated(double *calculated, double *expected) {}

		Get_Continuous_Levels,					//equivalent to ISignal::Get_Continuous_Levels
												//parameters[0].integer gives the number of parameters
												//parameters[1].integer gives the number of elements in the times and calculated arrays respectively
												//parameters[2].integer gives the desired degree of derivative
												//returns a function source with the following signature
												//void Get_Continous_Levels(double *parameters, double *times, double *calculated) {}
	}
		

	constexpr GUID IID_IFunction = { 0xde4d9967, 0x8b, 0x445d, { 0xb4, 0xe7, 0x3b, 0xb6, 0xf5, 0x80, 0x6e, 0x1 } };
	class IFunction : public virtual refcnt::IReferenced {
		/*
			For a given kernel_id, it returns specific function source so that it can be compiled with OpenCL 2.0 or later.
			The motivation is to compensate for the lack of OOP on GPU-like programming model.

			Segment is provided to resolve e.g., dependencies on other signals such Diffusion model depends on BG and IG.
			In addition, the OpenCL code can assume that it does not work on live data and hence segment can be used to cache
			those values, which will not change - e.g., results of ITimeSegments::Get_Discrete_Bounds.

			constant_memory may be already prefilled with some values, so the callee is supposed to add its constant
			data as needed - for example, measured signal will copy its data there. However, constants should be embeded
			into the function's source code to increase performance.
		*/
		HRESULT IfaceCalling Generate_Source(const NFunction_Id function_id, 
									 	     scgms::ITime_Segment *segment, const db::TParameter *parameters, const size_t parameter_count, 
											 refcnt::str_container *function_source,
											 refcnt::byte_container *constant_memory) = 0;
	};

}