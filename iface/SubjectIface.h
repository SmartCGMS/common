#pragma once

#include "SignalIface.h"

namespace glucose {

	class ITime_Segment : public virtual IReferenced {
		virtual HRESULT IfaceCalling Get_Signal(const TCalculation_Id *calc_id, IContinuous_Signal **signal);
			//calls AddRef on returned object

		virtual HRESULT IfaceCalling Get_Name(char *buffer, size_t count) = 0;
			//stores segment's name into the buffer as UTF-8, which holds count chars
			//returns ERROR_CONTINUE if the buffer is too small

		virtual HRESULT IfaceCalling Get_Stored_Params(TModel_Params *params) = 0;
			//loads stored params into *params, but params->calculation_id has to be already set!
		virtual HRESULT IfaceCalling Set_Stored_Params(const TModel_Params *params) = 0;
			//vice-versa

	};


	class ISubject : public virtual IReferenced {
		virtual HRESULT IfaceCalling Get_Time_Segment_By_Id(const size_t id, ITime_Segment **segment) = 0;
			/*
				id is a sesssion-only valid id,
				returns appropriate segment for this id into *segment
				Also, it calls AddRef() on the segment returned
			*/

		virtual HRESULT IfaceCalling Enumerate_Segment_Ids(const size_t *buf, const size_t bufsize, size_t *filled) = 0;
			//Eventually, we could return ERROR_CONTINUE if the buffer would be too small.
	};



	//ISubject_Storage is supposed to connect to ISensor to read data from it so that ITime_Segment will subsequently fetch and feed these data to IDiscrete_Glucose_Signal
	class ISubject_Storage : public virtual IReferenced {
		virtual HRESULT IfaceCalling Get_Segment_Name(const size_t segment_id, char *buffer, size_t count) = 0;
		virtual HRESULT IfaceCalling Get_Stored_Params(const size_t segment_id, const TModel_Params *params, size_t bufsize, size_t *filled) = 0;
			//if bufsize > 1, paramters other than for given segment_id are fetched (useful for genetic algorithms to obtain initial population)
		virtual HRESULT IfaceCalling Set_Stored_Params(const size_t segment_id, const TModel_Params *params) = 0;
			//and vice-versa
		virtual HRESULT IfaceCalling Enumerate_Segment_Ids(size_t *buf, size_t bufsize, size_t *filled) = 0;

	};

}