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

#pragma once

#include "FilterIface.h"
#include "../rtl/guid.h"

namespace scgms {

	/* flags for filter entities */
	enum class NFilter_Flags : uint8_t {
		None = 0,
		Encapsulated_Model = 1 << 0,        // the filter itself defines a model with the same GUID
		Presentation_Only = 1 << 1,         // the filter is used only during a presentation phase, i.e. it does not get instantiated during parameters optimalization and similar processes
	};

	using TFilter_Flags = std::underlying_type<NFilter_Flags>::type;

	inline NFilter_Flags operator|(const NFilter_Flags lhs, const NFilter_Flags rhs) {
		return static_cast<NFilter_Flags>(static_cast<TFilter_Flags>(lhs) | static_cast<TFilter_Flags>(rhs));
	}

	inline NFilter_Flags operator&(const NFilter_Flags lhs, const NFilter_Flags rhs) {
		return static_cast<NFilter_Flags>(static_cast<TFilter_Flags>(lhs) & static_cast<TFilter_Flags>(rhs));
	}

	/* descriptor of a filter entity */
	struct TFilter_Descriptor {
		/* the filter GUID */
		const GUID id;
		/* filter flags from the NFilter_Flags enumeration */
		const NFilter_Flags flags;
		/* filter string description (will be seen in user interface) */
		const wchar_t *description;
		/* count of filter parameters; can be zero */
		const size_t parameters_count;
		/* an array of filter parameter types; size is equal to parameters_count */
		const NParameter_Type* parameter_type;
		/* an array of filter parameter names as presented in user interface; size is equal to parameters_count */
		const wchar_t** ui_parameter_name;
		/* an array of filter parameter names as stored in config file; size is equal to parameters_count */
		const wchar_t** config_parameter_name;
		/* an array of filter parameter tooltips to be presented in user interface; size is equal to parameters_count; may be nullptr or contain nullptr where no tooltips required */
		const wchar_t** ui_parameter_tooltip;
	};
	
	constexpr TFilter_Descriptor Null_Filter_Descriptor = { Invalid_GUID, NFilter_Flags::None, nullptr, 0, nullptr, nullptr, nullptr, nullptr };

	/* descriptor of a metric entity */
	struct TMetric_Descriptor {
		/* metric GUID */
		const GUID id;
		/* metric string description (will be seen in user interface) */
		const wchar_t *description;
	};

	constexpr TMetric_Descriptor Null_Metric_Descriptor{ Invalid_GUID, nullptr };

	/* flags of a model */
	enum class NModel_Flags : uint8_t {
		None = 0,
		Signal_Model	= 1 << 0,
		Discrete_Model	= 1 << 1,
	};

	inline NModel_Flags operator|(const NModel_Flags lhs, const NModel_Flags rhs) {
		using TModel_Flags = std::underlying_type<NModel_Flags>::type;
		return static_cast<NModel_Flags>(static_cast<TModel_Flags>(lhs) | static_cast<TModel_Flags>(rhs));
	}

	inline NModel_Flags operator&(const NModel_Flags lhs, const NModel_Flags rhs) {
		using TModel_Flags = std::underlying_type<NModel_Flags>::type;
		return static_cast<NModel_Flags>(static_cast<TModel_Flags>(lhs) & static_cast<TModel_Flags>(rhs));
	}

	/* any model parameter must be expressed with double
	 * these constant express how to interpret that double */
	enum class NModel_Parameter_Value : int8_t {
		mptDouble,
		mptTime,
		mptBool
	};

	/* descriptor of a model entity */
	struct TModel_Descriptor {
		/* model GUID; this may be equal to a filter ID, if the filter embeds this model only; in this case, the filter must specify Encapsulated_Model flag */
		const GUID id;
		/* model flags from the NModel_Flags enumeration */
		const NModel_Flags flags;
		/* model string description (will be seen in user interface) */
		const wchar_t *description;
		/* database table name for this model (legacy; may be nullptr) */
		const wchar_t *db_table_name;

		/* total number of model parameters; must not be zero */
		const size_t total_number_of_parameters;
		/* number of model parameters, that are specific for every time segment; can be zero; always less or equal to total_number_of_parameters; these parameters are taken from the beginning of parameter array */
		const size_t number_of_segment_specific_parameters;
		/* an array of parameter types from the NModel_Parameter_Value enumeration; size of this array matches the total_number_of_parameters */
		const NModel_Parameter_Value *parameter_types;
		/* an array of parameter names as presented in user interface; size of this array matches the total_number_of_parameters */
		const wchar_t **parameter_ui_names;
		/* an array of parameter database column names (legacy); size of this array matches the total_number_of_parameters */
		const wchar_t **parameter_db_column_names;

		/* an array of parameter lower bounds; size of this array matches the total_number_of_parameters */
		const double *lower_bound;
		/* an array of parameter default values; default values must be between their lower_bound and upper_bound; size of this array matches the total_number_of_parameters */
		const double *default_values;
		/* an array of parameter upper bounds; size of this array matches the total_number_of_parameters */
		const double *upper_bound;

		/* a number of signals that are calculated by this model; cannot be zero */
		const size_t number_of_calculated_signals;	//cannot be zero
		/* an array of signal GUIDs that are produces by this model; size of this array matches the number_of_calculated_signals */
		const GUID* calculated_signal_ids;
		/* an array of reference signal GUIDs for the model calculated signals; size of this array matches the number_of_calculated_signals */
		const GUID* reference_signal_ids;

#ifdef __cplusplus
		inline TModel_Descriptor& operator= (const TModel_Descriptor& other) {
			std::memcpy(static_cast<void*>(this), &other, sizeof(TModel_Descriptor));
			return *this;
		}
#endif
	};

	constexpr TModel_Descriptor Null_Model_Descriptor = { Invalid_GUID, NModel_Flags::None, nullptr, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, nullptr, nullptr };

	/* descriptor of a solver entity */
	struct TSolver_Descriptor {
		/* solver GUID */
		const GUID id;
		/* solver string description (will be seen in user interface) */
		const wchar_t *description;

		/* is this solver specialized to some specific models? TRUE implies, that specialized_count is greater than 0 and the caller provides an array of model GUIDS
		 * FALSE means the solver is applicable to any model */
		const BOOL specialized;
		/* count of models the solver is specialized onto; applies only if specialized equals TRUE */
		const size_t specialized_count;
		/* an array of models this solver is specialized onto; size matches the specialized_count value */
		const GUID *specialized_models;
	};

	constexpr TSolver_Descriptor Null_Solver_Descriptor = { Invalid_GUID, nullptr, false, 0, nullptr };

	/* descriptor of a signal approximator entity */
	struct TApprox_Descriptor {
		/* approximator GUID */
		const GUID id;
		/* approximator string description (will be seen in user interface) */
		const wchar_t *description;
	};

	constexpr TApprox_Descriptor Null_Approx_Descriptor = { Invalid_GUID, nullptr };

	/* enumeration of units known to SmartCGMS; these are applied to signals and their values */
	enum class NSignal_Unit : uint16_t {
		Unitless = 0,     // no units
		Percent,          // percents
		datetime,         // rattime (encoded as the number of days since January 0, 1900 00:00 UTC, see http://en.wikipedia.org/wiki/January_0 )
		mmol_per_L =3,    // mmol/l - instead of mol/L due to historical reasons
		U_insulin,        // insulin units, not UI nor IU
		U_per_Hr,         // insulin units per hour
		Celsius,          // degrees in Celsius scale
		BPM,              // beats per minute, e.g., for heart rate
		S,                // Siemens e.g., for for galvanic skin response aka electrodermal activity
		A,                // ampers
		m_per_s,          // meters per second, e.g., for speed
		m_per_s2,         // meters per second squared, e.g., for acceleration
		g,                // grams - instead of SI kg due to historical reasons
		Hz,               // Hertz
		Calories,         // calories

		Other             // unit not described here, but its description is given in the signal descriptor
	};

	/* a type of signal visualization */
	enum class NSignal_Visualization : uint8_t {
		smooth = 0,         // the signal curve should be smoothed, e.g., spline interpolation between two sampled values
		step,               // step function
		mark,               // display marks only at each sampled value
		step_with_mark,     // step function with marks
		smooth_with_mark,   // curve smoothing with marks
	};

	/* a type of signal mark to be drawn during visualization */
	enum class NSignal_Mark : char {
		none = 0,
		space = ' ', // special value for setting up a stroke pattern
		dot = '.',
		plus = '+',
		minus = '-',
		star = '*',
		rectangle = 'r',
		diamond = 'd',
		cross = 'x',
		circle = 'o',
		triangle = 't',
	};

	/* descriptor of a signal entity */
	struct TSignal_Descriptor {
		/* signal GUID */
		const GUID id;
		/* signal string description (will be seen in user interface) */
		const wchar_t* signal_description;
		/* signal unit description (will be seen in visualizations) */
		const wchar_t* unit_description;
		/* unit identifier from NSignal_Unit enumeration */
		const NSignal_Unit unit_id;
		/* color which will be used to fill the curve (ARGB numeric format) */
		const uint32_t fill_color;
		/* color which will be used to stroke the curve (ARGB numeric format) */
		const uint32_t stroke_color;
		/* a type of signal visualization from NSignal_Visualization enumeration */
		const NSignal_Visualization visualization;
		/* a type of mark from NSignal_Mark enumeration */
		const NSignal_Mark mark;
		/* an array of stroke patterns for e.g., dashed lines, etc.; may be nullptr for solid line; if not, always terminated with NSignal_Mark::none */
		const NSignal_Mark *stroke_pattern;
		/* scale of visualized values (e.g., if the signal reads 10 and value_scale is 0.1, it visually matches the value of 1 of other signals) */
		const double value_scale;
	};

	constexpr TSignal_Descriptor Null_Signal_Descriptor = { Invalid_GUID, nullptr, nullptr, NSignal_Unit::Unitless, 0, 0, NSignal_Visualization::smooth, NSignal_Mark::none, nullptr, 1.0 };

	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);
	using TGet_Metric_Descriptors = HRESULT(IfaceCalling*)(TMetric_Descriptor **begin, TMetric_Descriptor **end);
	using TGet_Model_Descriptors = HRESULT(IfaceCalling*)(TModel_Descriptor **begin, TModel_Descriptor **end);
	using TGet_Solver_Descriptors = HRESULT(IfaceCalling*)(TSolver_Descriptor **begin, TSolver_Descriptor **end);
	using TGet_Approx_Descriptors = HRESULT(IfaceCalling*)(TApprox_Descriptor **begin, TApprox_Descriptor **end);
	using TGet_Signal_Descriptors = HRESULT(IfaceCalling*)(TSignal_Descriptor **begin, TSignal_Descriptor **end);
}
