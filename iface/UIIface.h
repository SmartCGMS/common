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

	
	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);
	using TGet_Metric_Descriptors = HRESULT(IfaceCalling*)(TMetric_Descriptor **begin, TMetric_Descriptor **end);
	using TGet_Model_Descriptors = HRESULT(IfaceCalling*)(TModel_Descriptor **begin, TModel_Descriptor **end);
	using TGet_Solver_Descriptors = HRESULT(IfaceCalling*)(TSolver_Descriptor **begin, TSolver_Descriptor **end);
	using TGet_Approx_Descriptors = HRESULT(IfaceCalling*)(TApprox_Descriptor **begin, TApprox_Descriptor **end);

}

