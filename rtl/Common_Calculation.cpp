#include "Common_Calculation.h"

CCommon_Calculation::CCommon_Calculation(glucose::WTime_Segment segment, const GUID &source_signal) : mSource_Signal(segment.Get_Signal(source_signal)) {
	if (!refcnt::Shared_Valid_All(mSource_Signal)) throw std::exception{};
}

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const {
	return E_NOTIMPL;
}

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Bounds(glucose::TBounds *bounds, size_t *level_count) const {
	return E_NOTIMPL;
}

HRESULT IfaceCalling CCommon_Calculation::Add_Levels(const double *times, const double *levels, const size_t count) {
	return E_NOTIMPL;	//we calculate from a signal that can possibly add new levels
}