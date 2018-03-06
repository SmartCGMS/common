#pragma once


#include "DeviceLib.h"
#include "referencedImpl.h"
#include "Buffer_Pool.h"
#include "Eigen_Buffer_Pool.h"


#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance


class CCommon_Calculation : public virtual glucose::ISignal, public virtual refcnt::CReferenced {
protected:
	template <typename T>
	T& Convert_Parameters(glucose::IModel_Parameter_Vector *params, const double *default_parameters) const {
		double *begin{ const_cast<double*>(default_parameters) }, *end;	//{default_params} to keep compiler happy that we don't have potentially uninitialized variable as compile (and us in-fact) have no guarantee that ->get will set it right
		if (params) params->get(&begin, &end);		//we don't care about the (h)result as begin is already initialized to default parameters
	
		T &result = *(reinterpret_cast<T*>(begin));
		return result;
	}
protected:
	mutable CBuffer_Pool<TVector1D> mVector1D_Pool{Eigen_Buffer_Pool_Resize<TVector1D> };
protected:
	glucose::SSignal mReference_Signal;
public:	
	CCommon_Calculation() {};
	CCommon_Calculation(glucose::WTime_Segment segment, const GUID &reference_signal);
	virtual ~CCommon_Calculation() {};

	//glucose::ISignal iface
	virtual HRESULT IfaceCalling Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const final;
	virtual HRESULT IfaceCalling Get_Discrete_Bounds(glucose::TBounds *bounds, size_t *level_count) const final;
	virtual HRESULT IfaceCalling Add_Levels(const double *times, const double *levels, const size_t count) final;
	
};

#pragma warning( pop )