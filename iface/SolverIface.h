#pragma once

#include "DeviceIface.h"

namespace glucose {

	


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
		virtual HRESULT IfaceCalling Accumulate(const double *times, const double *reference, const double *calculated, const size_t count) = 0;
			//let's the calculator to process a batch of known differences
			//count is the number of elements of differences, which are encoded as vectors to exploit SIMD
			//		this will becaome significant with ist prediction, where increased number of levels is expected compared to blood
			//count is the total number of all levels that could have been calculated under optimal conditions
			//		not calculated levels are quiet NaN
		virtual HRESULT IfaceCalling Reset() = 0;
			//undo all previously called Accumulate
		virtual HRESULT IfaceCalling Calculate(double *metric, size_t *levels_accumulated, size_t levels_required) = 0;
			//calculates the metric - the less number is better
			//levels will be the number of levels accumulated
			//returns S_FALSE if *levels_accumulated < levels_required

		virtual HRESULT IfaceCalling Get_Parameters(TMetric_Parameters *parameters) = 0;
			//so that we can clone the metric, e.g., over a network connection
	};



	struct TSolver_Progress {
			//solver sets these values to indicate its progress
		size_t current_progress, max_progress;	//minimum progress is zero
		double best_metric;		
		char cancelled;	//just cast it to bool, if set to true, solver cancels the current operation				   			
	};


	struct TSolver_Setup {
		const GUID solver_id; const GUID calculated_signal_id; const GUID reference_signal_id;
		ITime_Segment **segments; const size_t segment_count;
		IMetric *metric; const size_t levels_required; const char use_measured_levels;
		IModel_Parameter_Vector *lower_bound, *upper_bound; 
		IModel_Parameter_Vector **solution_hints; const size_t hint_count;
		IModel_Parameter_Vector *solved_parameters;		//obtained result
		TSolver_Progress *progress;
	};


	using TCreate_Metric = HRESULT(IfaceCalling*)(const TMetric_Parameters *parameters, IMetric **metric);
	using TSolve_Model_Parameters = HRESULT(IfaceCalling*)(const TSolver_Setup *setup);
		//generic, e.g., evolutionary, solver uses signal_id to calculate its metric function on the given list of segments
		//specialized solver has the signal ids encoded - i.e., specialized inside		
		//the very first hint, if provided, has to be the best one
	
}