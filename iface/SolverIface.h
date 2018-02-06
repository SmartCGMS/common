#pragma once

#include "DeviceIface.h"

namespace glucose {

	struct TDifference_Point {
		double expected;
		double calculated;
		double datetime;		//datetime when the level was measured
	} ;


	struct TMetric_Parameters {
		const GUID metric_id;
			//any metric can ignore the parameters below as seen fit as e.g., AIC or Leal_2010 are not compatible with all the options
		const unsigned char use_relative_error;
		const unsigned char use_squared_differences;
		const unsigned char prefer_more_levels;		//i.e. result is once more divided by the number of levels evaluated
		const double threshold;					//particular meaning depends on used metric and the caller is responsible for providing correct value
	};

	class IMetric: public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Accumulate(const TDifference_Point *begin, const TDifference_Point *end, size_t all_levels_count) = 0;
			//let's the calculator to process a batch of known differences
			//count is the number of elements of differences
			//alllevelscount is the total number of all levels that could have been calculated under optimal conditions
		virtual HRESULT IfaceCalling Reset() = 0;
			//undo all previously called Accumulate
		virtual HRESULT IfaceCalling Calculate(double *metric, size_t *levels_accumulated, size_t levels_required) = 0;
			//calculates the metric - the less number is better
			//levels will be the number of levels accumulated
			//returns S_FALSE if *levels_accumulated < levels_required

		virtual HRESULT IfaceCalling Get_Parameters(TMetric_Parameters *parameters) = 0;
			//so that we can clone the metric, e.g., over a network connection
	};



	using TCreate_Metric = HRESULT(IfaceCalling*)(const GUID *metric_id, const TMetric_Parameters *parameters, IMetric **metric);
	using TSolve_Model_Parameters = HRESULT(IfaceCalling*)(const GUID *solver_id, const GUID *signal_id, const ITime_Segment **segments, const size_t segment_count, IMetric *metric, 
														  TModel_Parameter_Vector *lower_bound, TModel_Parameter_Vector *upper_bound, TModel_Parameter_Vector **solved_parameters);
		//generic, e.g., evolutionary, solver uses signal_id to calculate its metric function on the given list of segments
		//specialized solver has the signal ids encoded - i.e., specialized inside		
	
}