#pragma once


#include "DeviceLib.h"
#include "referencedImpl.h"


#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance


class CCommon_Calculation : public virtual glucose::ISignal, public virtual refcnt::CReferenced {
public:	
	virtual ~CCommon_Calculation() {};

	//glucose::ISignal iface
	virtual HRESULT IfaceCalling Get_Discrete_Levels(const double *times, const double *levels, const size_t count, size_t *filled) const final;
	virtual HRESULT IfaceCalling Get_Discrete_Bounds(glucose::TBounds *bounds, size_t *level_count) const final;
	virtual HRESULT IfaceCalling Add_Levels(const double *times, const double *levels, const size_t count) final;
	
};

#pragma warning( pop )