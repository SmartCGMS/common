#pragma once

#include "../iface/SolverIface.h"

#include <memory>

namespace glucose {
	
	class SMetric : public std::shared_ptr<IMetric> {
	private:
		void Init(const glucose::TMetric_Parameters &params);
	public:
		SMetric();
		SMetric(const TMetric_Parameters &params);
		SMetric Clone();
	};


	class IErrors_Filter : public glucose::IFilter, public virtual refcnt::CReferenced {
	public:

		// retrieves error container for given signal and error type
		virtual HRESULT IfaceCalling Get_Errors(const GUID& signal_id, glucose::TError_Container& target, glucose::NError_Type type) = 0;
	};


	using SErrors_Filter = std::shared_ptr<IErrors_Filter>;

	constexpr GUID Errors_Filter = { 0x4a125499, 0x5dc8, 0x128e,{ 0xa5, 0x5c, 0x14, 0x22, 0xbc, 0xac, 0x10, 0x74 } };

	using TGet_Errors = HRESULT(IfaceCalling*)(const GUID*, glucose::TError_Container*, glucose::NError_Type);

}