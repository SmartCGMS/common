#pragma once

#include <cstdint>

namespace glucose {

	typedef uint8_t TModel_Id;
	typedef uint8_t TSignal_Id;
	typedef uint8_t TMethod_Id;
	typedef uint8_t TFlags;

	typedef struct {
		TModel_Id  model_id;
		TSignal_Id signal_id;
		TMethod_Id method_id;
		TFlags flags;
	} TCalculation_Id;


	constexpr static TModel_Id  mdlInvalid = 0;
	constexpr static TModel_Id  mdlMeasurement = 1;
	constexpr static TModel_Id  mdlDiffusion_2 = 2;
	constexpr static TModel_Id  mdlDiffusion_4 = 3;
	constexpr static TModel_Id  mdlDiffusion_Ist_Prediction = 4;
	constexpr static TModel_Id  mdlWeighted_Moving_Average = 5;
	constexpr static TModel_Id  mdlSteil_Rebrin = 6;

	constexpr static TSignal_Id sglInvalid = 0;
	constexpr static TSignal_Id sglBlood = 1;
	constexpr static TSignal_Id sglInterstitial = 2;
	constexpr static TSignal_Id sglISIG = 3;
	constexpr static TSignal_Id sglCalibration = 4;

	constexpr static TMethod_Id mtdInvalid = 0;
	constexpr static TMethod_Id mtdAnalytic = 1;
	constexpr static TMethod_Id mtdMetaDE = 2;
	constexpr static TMethod_Id mtdNewUOA = 3;
	constexpr static TMethod_Id mtdBOBYQA = 4;


	typedef struct {
		constexpr static size_t count = 4;
		union {
			struct {
				floattype p, cg, c, dt;
			};
			floattype vector[count];
		};
	} TDiffusion_1_Params;


	typedef struct {
		constexpr static size_t count = 6;
		union {
			struct {
				floattype p, cg, c, dt, k, h;
			};
			floattype vector[count];
		};
	} TDiffusion_2_Params;

	typedef struct {
		constexpr static size_t count = 6;
		union {
			struct {
				floattype p, cg, c;
				floattype measured_dt, future_dt;
			};
			floattype vector[count];
		};
	} TDiffusion_4_Params;

	typedef struct {
		floattype weight;
		floattype delta;
	} TWeight_Delta;



	typedef struct {
		constexpr static size_t n = 2;
		constexpr static size_t count = 2+n;
		union {
			struct {
				floattype future_delta;
				floattype c;
				TWeight_Delta weight[n];
			};
			floattype vector[count];
		};
	} TWeighted_Moving_Average_Params;

	typedef struct {
		constexpr static size_t count = 2 * TDiffusion_1_Params::count+2;
		union {
			struct {
				TDiffusion_1_Params retrospective, predictive;
				floattype correction_c, correction_k;
			};
			floattype vector[count];
		};
	} TDiffusion_Ist_Prediction;

	typedef struct {
		constexpr static size_t count = 4;
		union {
			struct {
				floattype tau,;
				floattype alpha, beta, gamma;
			};
			floattype vector[count];
		};
	} TSteil_Rebrin_Params;

	typedef struct {
		TCalculation_Id calculation_id;
		union {
			TDiffusion_1_Params diffusion_1;
			TDiffusion_2_Params diffusion_2;
			TDiffusion_4_Params diffusion_4;
			TDiffusion_Ist_Prediction diffusion_ist_prediction;
			TSteil_Rebrin_Params steil_rebrin;
		};
	} TModel_Params;

}