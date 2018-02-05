#pragma once

#include "DeviceIface.h"

namespace glucose {

	struct TDifference_Point {
		double expected;
		double calculated;
		double datetime;		//datetime when the level was measured
	} ;


	//Standard Metric  IDs

	static constexpr GUID mtrAvg_Abs =	//arithmetic average absolute error
	{ 0xd272a84d, 0x50ff, 0x46ce,{ 0x97, 0x7e, 0xc8, 0xe3, 0x68, 0xc3, 0x70, 0x6a } }; 	// {D272A84D-50FF-46CE-977E-C8E368C3706A}

	static constexpr GUID mtrMax_Abs =		//maximum absolute error
	{ 0x70a34eac, 0xac0a, 0x424f, { 0xa3, 0x2c, 0x20, 0xfd, 0x51, 0x7b, 0xec, 0xe6 } };	// {70A34EAC-AC0A-424F-A32C-20FD517BECE6}

	static constexpr GUID mtrPerc_Abs =		////error at percentil given by TMetricParameters.Threshold (e.g. 25% is 25.0)
	{ 0x3c3b19d, 0x4a2, 0x4195, { 0x8a, 0x77, 0x7f, 0xed, 0xa7, 0x8a, 0x95, 0x39 } };	// {03C3B19D-04A2-4195-8A77-7FEDA78A9539}
																						
	static constexpr GUID mtrThresh_Abs =	//number of levels with error greater than TMetricParameters.Threshold (e.g. 20% is 20.0)
	{ 0x9ca7aa77, 0xc84b, 0x4998,{ 0xbd, 0x8f, 0xd, 0xe0, 0x9, 0x67, 0xb0, 0x8e } };	// {9CA7AA77-C84B-4998-BD8F-0DE00967B08E}
																						
	static constexpr GUID mtrLeal_2010 =	//adapted best fit from Yenny Leal at al. Real-Time Glucose Estimation Algorithm for Continuous Glucose Monitoring Using Autoregressive Models
	{ 0xe6d43190, 0x4317, 0x4201,{ 0x87, 0x6a, 0xf, 0xe4, 0xfe, 0x6b, 0x9, 0x90 } };	// {E6D43190-4317-4201-876A-0FE4FE6B0990}
																						
	static constexpr GUID mtrAIC =			//Akaike Information Criterion
	{ 0x56286dd1, 0x6d95, 0x4c29,{ 0x9c, 0xda, 0x2b, 0x78, 0xbf, 0x7e, 0x5d, 0x16 } };	// {56286DD1-6D95-4C29-9CDA-2B78BF7E5D16}
																			
	static constexpr GUID mtrStd_Dev =	//standard deviation, TMetricParameters.Threshold gives percentils (e.g. 5% is 5.0) to cut off from the beginning and the end of sorted differences
	{ 0x6c8b6358, 0xdf3e, 0x45e4,{ 0xae, 0x39, 0x9a, 0x55, 0x34, 0x7d, 0xa5, 0x85 } };	// {6C8B6358-DF3E-45E4-AE39-9A55347DA585}
																						
	static constexpr GUID mtrCrosswalk =	// the Croswalk - time-order metric from EMBEC 2017
	{ 0x1feed1ce, 0x4ab3, 0x42be,{ 0x83, 0x34, 0x77, 0x46, 0x80, 0x27, 0xf, 0x14 } };	// {1FEED1CE-4AB3-42BE-8334-774680270F14}
																						
	static constexpr GUID mtrIntegral_CDF =	//integrates the area beneath cumulative distribution function of error - replaces the standard Area Under the Curve
	{ 0x7b869d40, 0xb8a5, 0x4109,{ 0x87, 0xac, 0xe4, 0xd9, 0xd7, 0x2b, 0x99, 0x8a } };	// {7B869D40-B8A5-4109-87AC-E4D9D72B998A}
																							
	static constexpr GUID mtrAvg_Plus_Bessel_Std_Dev =	//average plus standard deviation with Bessel's correction
	{ 0x5faaa53b, 0x2e58, 0x4e0e,{ 0x8b, 0xd0, 0x2c, 0x79, 0xde, 0x90, 0xfe, 0xbb } };	// {5FAAA53B-2E58-4E0E-8BD0-2C79DE90FEBB}



	struct TMetric_Parameters {
		GUID metric_id;
			//any metric can ignore the parameters below as seen fit as e.g., AIC or Leal_2010 are not compatible with all the options
		unsigned char use_relative_error;
		unsigned char use_squared_differences;
		unsigned char prefer_more_levels;		//i.e. result is once more divided by the number of levels evaluated
		double threshold;					//particular meaning depends on used metric and the caller is responsible for providing correct value
	};

	class IMetric: public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Accumulate(TDifference_Point *differences, size_t count, size_t all_levels_count) = 0;
			//let's the calculator to process a batch of known differences
			//count is the number of elements of differences
			//alllevelscount is the total number of all levels that could have been calculated under optimal conditions
		virtual HRESULT IfaceCalling Reset() = 0;
			//undo all previously called Accumulate
		virtual HRESULT IfaceCalling Calculate(floattype *metric, size_t *levels_accumulated, size_t levels_required) = 0;
			//calculates the metric - the less number is better
			//levels will be the number of levels accumulated
			//returns S_FALSE if *levelsaccumulated < levelsrequired

		virtual HRESULT IfaceCalling Get_Parameters(TMetric_Parameters *parameters) = 0;
			//so that we can clone the metric, e.g., over a network connection
	};



	using TCreate_Metric = HRESULT(IfaceCalling*)(const GUID *metric_id, const TMetric_Parameters *parameters, IMetric **metric);
	using TSolve_Model_Parameters = HRESULT(IfaceCalling*)(const GUID *solver_id, const GUID *signal_id, const ITime_Segment **segments, const size_t segment_count, IMetric *metric, 
														  TModel_Parameter_Vector *lower_bound, TModel_Parameter_Vector *upper_bound, TModel_Parameter_Vector **solved_parameters);
		//generic, e.g., evolutionary, solver uses signal_id to calculate its metric function on the given list of segments
		//specialized solver has the signal ids encoded - i.e., specialized inside		
	
}