#pragma once

#include "FilterIface.h"
#include "../rtl/guid.h"

namespace glucose {

	//Units used for glucose levels 
	enum class TUnits : int8_t {
		MmolPerL = 0, //mmol/l
		MgPerDl,	//mg/dl
		DgPerDl,	//dg/dl
	};


	struct TFilter_Descriptor {
		const GUID id;
		const wchar_t *description;
		const size_t parameters_count;	//can be zero
		const NParameter_Type* parameter_type;
		const wchar_t** ui_parameter_name;
		const wchar_t** config_parameter_name;
		const wchar_t** ui_parameter_tooltip; // always the same size as other parameter fields, nullptr where no tooltip needed
	};


	struct TMetric_Descriptor {		
		const GUID id;
		const wchar_t *description;
	};


	enum class NModel_Parameter_Value : int8_t {
		//any model parameter must be expressed with double
		//these constant express how to interpret that double

		mptDouble,
		mptTime,
		mptBool
	};

	struct TModel_Descriptor {
		const GUID id;
		const wchar_t *description;
		const wchar_t *db_table_name;

		const size_t number_of_parameters;	//cannot be zero
		const NModel_Parameter_Value *parameter_types;		//array of mptConstants
		const wchar_t **parameter_ui_names;
		const wchar_t **parameter_db_column_names;

											//default values
		const double *lower_bound;
		const double *default_values;
		const double *upper_bound;

		//signals which can be calculated using this model
		const size_t number_of_calculated_signals;	//cannot be zero
		const GUID* calculated_signal_ids;			
		const wchar_t **calculated_signal_names;
		const GUID* reference_signal_ids;
	};


	struct TSolver_Descriptor {
		const GUID id;
		const wchar_t *description;

		const bool specialized;	//if false, can be applied to any model
								//if true, the following fields apply
		const size_t specialized_count;		//can be zero with specialized == false, usually 1
		const GUID *specialized_models;		//array of models, whose parameters the solver can solve
	};

	struct TApprox_Descriptor {
		const GUID id;
		const wchar_t *description;
		const size_t parameters_count;	//can be zero
		const wchar_t** ui_parameter_name;
		const wchar_t** config_parameter_name;
	};

	enum class Error_Misc_Type : size_t
	{
		Average = 0,
		StdDev,
		AIC
	};

	enum class Error_Quantile_Type : size_t
	{
		Minimum = 0,
		Q1,
		Median,
		Q3,
		Q95,
		Q99,
		Maximum
	};

	constexpr size_t Error_Type_Count = 2;
	constexpr size_t Error_Quantile_Count = 7;
	constexpr size_t Error_Misc_Count = 3;

	// Structure for containing error metric values
	struct TError_Container
	{
		union
		{
			double misc[Error_Misc_Count];
			struct
			{
				double avg;						// average value
				double stddev;					// standard deviation with Bessel's correction
				double aic;						// Akaike's information criterion
			};
		};
		union
		{
			double q[Error_Quantile_Count]; // min, q1, median, q3, q95, q99, max
			struct
			{
				double minval;					// minimum value (0. percentile)
				double q1;						// 1. quartile (25. percentile)
				double median;					// median (50. percentile)
				double q3;						// 3. quartile (75. percentile)
				double q95;						// 95. percentile
				double q99;						// 99. percentile
				double maxval;					// maximum value ("100." percentile)
			};
		};
	};

	// error types
	enum class NError_Type
	{
		Absolute = 0,
		Relative = 1
	};



	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);
	using TGet_Metric_Descriptors = HRESULT(IfaceCalling*)(TMetric_Descriptor **begin, TMetric_Descriptor **end);
	using TGet_Model_Descriptors = HRESULT(IfaceCalling*)(TModel_Descriptor **begin, TModel_Descriptor **end);
	using TGet_Solver_Descriptors = HRESULT(IfaceCalling*)(TSolver_Descriptor **begin, TSolver_Descriptor **end);
	using TGet_Approx_Descriptors = HRESULT(IfaceCalling*)(TApprox_Descriptor **begin, TApprox_Descriptor **end);

}

