#pragma once

#include "../rtl/guid.h"
#include "referencedIface.h"
#include "DeviceIface.h"

namespace glucose
{
	using IApprox_Parameters_Vector = refcnt::double_container;

	class IApproximator : public virtual refcnt::IReferenced
	{
		public:
			// parameters of approximation curve(s) on given signal (signal is passed in constructor of each approximation) are calculated when
			// performing construction of approximator object;

			// approximates signal levels (or derivations) on given times array of size "count"
			// a call to Approximate method must precede (once per ISignal data set)
			virtual HRESULT IfaceCalling GetLevels(const double* times, double* const levels, const size_t count, const size_t derivation_order) = 0;
	};

	using TCreate_Approximator = HRESULT(IfaceCalling *)(const GUID *approx_id, ISignal *signal, IApproximator **approx, IApprox_Parameters_Vector* configuration);
}
