#pragma once

#include "SignalIface.h"

namespace glucose {

	class ITime_Segment : public virtual IReferenced {
		virtual HRESULT IfaceCalling Get_Signal(const TCalculation_Id *calc_id, IContinuous_Signal **signal);
			//calls AddRef on returned object

		virtual HRESULT IfaceCalling GetName(char *buffer, size_t count) = 0;
			//stores segment's name into the buffer as UTF-8, which holds count chars
			//returns ERROR_CONTINUE if the buffer is too small

		virtual HRESULT IfaceCalling GetStoredParams(TModel_Params *params) = 0;
			//loads stored params into *params, but params->calculation_id has to be already set!
		virtual HRESULT IfaceCalling SetStoredParams(const TModel_Params *params) = 0;
			//vice-versa

	};

}