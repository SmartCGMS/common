#include "Common_Calculation.h"

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Levels(const double *times, const double *levels, const size_t count, size_t *filled) const {
	return E_NOTIMPL;	//What if the model uses multiple input signals? What are references times and levels then?
						//Remember that the signals do not need to be synchronized at all.
}

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Bounds(glucose::TBounds *bounds, size_t *level_count) const  {
	return E_NOTIMPL;	//cannot calculate bounds without model parameters 
						//and if we add one than we break API with measured signals
						//but more importantly - see the dilema at Get_Discrete_Levels
};

HRESULT IfaceCalling CCommon_Calculation::Add_Levels(const double *times, const double *levels, const size_t count) {
	return E_NOTIMPL;	//we calculate from a signal that can possibly add new levels
}
