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

#include "FilterIface.h"
#include "../rtl/guid.h"

namespace scgms {

	enum class NFilter_Flags : uint8_t {
		None = 0
	};

	using TFilter_Flags = std::underlying_type<NFilter_Flags>::type;

	inline NFilter_Flags operator|(const NFilter_Flags lhs, const NFilter_Flags rhs) {
		return static_cast<NFilter_Flags>(static_cast<TFilter_Flags>(lhs) | static_cast<TFilter_Flags>(rhs));
	}

	inline NFilter_Flags operator&(const NFilter_Flags lhs, const NFilter_Flags rhs) {
		return static_cast<NFilter_Flags>(static_cast<TFilter_Flags>(lhs) & static_cast<TFilter_Flags>(rhs));
	}

	struct TFilter_Descriptor {
		const GUID id;
		const NFilter_Flags flags;
		const wchar_t *description;
		const size_t parameters_count;	//can be zero
		const NParameter_Type* parameter_type;
		const wchar_t** ui_parameter_name;
		const wchar_t** config_parameter_name;
		const wchar_t** ui_parameter_tooltip; // always the same size as other parameter fields, nullptr where no tooltip needed
	};
	
	constexpr TFilter_Descriptor Null_Filter_Descriptor = { Invalid_GUID, NFilter_Flags::None, nullptr, 0, nullptr, nullptr, nullptr, nullptr };

	struct TMetric_Descriptor {
		const GUID id;
		const wchar_t *description;
	};

	enum class NModel_Flags : uint8_t {
		None = 0,
		Signal_Model	= 1 << 0,
		Discrete_Model	= 1 << 1,
	};

	using TModel_Flags = std::underlying_type<NModel_Flags>::type;

	inline NModel_Flags operator|(const NModel_Flags lhs, const NModel_Flags rhs) {
		return static_cast<NModel_Flags>(static_cast<TModel_Flags>(lhs) | static_cast<TModel_Flags>(rhs));
	}

	inline NModel_Flags operator&(const NModel_Flags lhs, const NModel_Flags rhs) {
		return static_cast<NModel_Flags>(static_cast<TModel_Flags>(lhs) & static_cast<TModel_Flags>(rhs));
	}

	/* any model parameter must be expressed with double
	   these constant express how to interpret that double
	*/
	enum class NModel_Parameter_Value : int8_t {
		mptDouble,
		mptTime,
		mptBool
	};

	struct TModel_Descriptor {
		const GUID id;
		const NModel_Flags flags;
		const wchar_t *description;
		const wchar_t *db_table_name;

		const size_t number_of_parameters;				//cannot be zero
		const NModel_Parameter_Value *parameter_types;	//array of mptConstants
		const wchar_t **parameter_ui_names;
		const wchar_t **parameter_db_column_names;

		const double *lower_bound;
		const double *default_values;
		const double *upper_bound;

		//signals which can be calculated using this model
		const size_t number_of_calculated_signals;	//cannot be zero
		const GUID* calculated_signal_ids;
		const GUID* reference_signal_ids;
	};

	constexpr TModel_Descriptor Null_Model_Descriptor = { Invalid_GUID, NModel_Flags::None, nullptr, nullptr, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr };

	struct TSolver_Descriptor {
		const GUID id;
		const wchar_t *description;

		const BOOL specialized;	//if false, can be applied to any model
								//if true, the following fields apply
		const size_t specialized_count;		//can be zero with specialized == false, usually 1
		const GUID *specialized_models;		//array of models, whose parameters the solver can solve
	};

	constexpr TSolver_Descriptor Null_Solver_Descriptor = { Invalid_GUID, nullptr, false, 0, nullptr };

	struct TApprox_Descriptor {
		const GUID id;
		const wchar_t *description;
		const size_t parameters_count;	//can be zero
		const wchar_t** ui_parameter_name;
		const wchar_t** config_parameter_name;
	};

	constexpr TApprox_Descriptor Null_Approx_Descriptor = { Invalid_GUID, nullptr, 0, nullptr, nullptr };

	//Units used for displayed signals
	enum class NSignal_Unit : uint16_t {
		Unitless = 0,
		Percent,
		datetime,	//The time is encoded as the number of days since January 0, 1900 00:00 UTC, see http://en.wikipedia.org/wiki/January_0
		mmol_per_L, //mmol/l - instead of mol/L due to historical reasons
		U_insulin,	//insulin units, not UI nor IU
		U_per_Hr,
		Celsius,
		BPM,		//beats per minute, e.g.; heart rate
		S,			//Siemens e.g.; for galvanic skin response aka electrodermal activity
		A,			//nano ampers
		m_per_s2,	//acceleration
		g,			//grams - - instead of SI kg due to historical reasons

		Other		//a unit not described here, but its description is given in the signal descriptor
	};


	enum class NSignal_Visualization : uint8_t {
		smooth = 0,		//e.g.; spline interpolation between two sampled values
		step,			//step function
		mark,			//display marks only at each sampled value
		step_with_mark,
		smooth_with_mark
	};

	enum class NSignal_Mark : char {
		none = 0,		
		space = ' ',		//for setting up a stroke pattern
		dot = '.',
		plus = '+',
		minus = '-',
		star = '*',
		rectangle = 'r',
		diamond = 'd',
		cross = 'x',
		circle = 'o',
		triangle = 't'
	};

	struct TSignal_Descriptor {		//designates preferred choices to display/desribe the signal
		const GUID id;
		const wchar_t* signal_description;
		const wchar_t* unit_description;
		const NSignal_Unit unit_id;
		const uint32_t fill_color, stroke_color;	//ARGB
		const NSignal_Visualization visualization;
		const NSignal_Mark mark;
		const NSignal_Mark *stroke_pattern;	//NSignal_Mark::none terminated stroke pattern, nullptr means solid line
	};

	constexpr TSignal_Descriptor Null_Signal_Descriptor = { Invalid_GUID, nullptr, nullptr, NSignal_Unit::Unitless, 0, 0, NSignal_Visualization::smooth, NSignal_Mark::none, nullptr};

	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);
	using TGet_Metric_Descriptors = HRESULT(IfaceCalling*)(TMetric_Descriptor **begin, TMetric_Descriptor **end);
	using TGet_Model_Descriptors = HRESULT(IfaceCalling*)(TModel_Descriptor **begin, TModel_Descriptor **end);
	using TGet_Solver_Descriptors = HRESULT(IfaceCalling*)(TSolver_Descriptor **begin, TSolver_Descriptor **end);
	using TGet_Approx_Descriptors = HRESULT(IfaceCalling*)(TApprox_Descriptor **begin, TApprox_Descriptor **end);
	using TGet_Signal_Descriptors = HRESULT(IfaceCalling*)(TSignal_Descriptor **begin, TSignal_Descriptor **end);

	using TAdd_Filters = HRESULT(IfaceCalling *)(const scgms::TFilter_Descriptor *begin, const scgms::TFilter_Descriptor *end, const scgms::TCreate_Filter create_filter);
}
