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
		double *begin{ const_cast<double*>(default_parameters) };	//just in case that no parameters are set at all -> than we have to use the default ones
		if (params) {
			double *tmp_begin, *end;
			if (params->get(&tmp_begin, &end) == S_OK) {		
				//not that params still could be empty
				if (tmp_begin && (tmp_begin != end))  begin = tmp_begin;
			}
		}
	
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
	virtual HRESULT IfaceCalling Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const override;
	virtual HRESULT IfaceCalling Get_Discrete_Bounds(glucose::TBounds *bounds, size_t *level_count) const override;
	virtual HRESULT IfaceCalling Add_Levels(const double *times, const double *levels, const size_t count) override;
	
};

#pragma warning( pop )