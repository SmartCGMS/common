#pragma once

#include "..\iface\DeviceIface.h"
#include "referencedImpl.h"
#include "guid.h"

#include <memory>
#include <map>
#include <vector>

namespace glucose {

	using SModel_Parameter_Vector = std::shared_ptr<IModel_Parameter_Vector>;

	using SSignal = std::shared_ptr<ISignal>;

	class WTime_Segment { //: public std::weak_ptr<ITime_Segment> { --cannot inherit because shared-weak relationshop cannot live across COM interface
						  //Perhaps, we should make STime_Segment export friendly function to WTimeSegment that will take WTime_Segment's
						  //as callback of STime_Segment. STime_Segment will call the WTime_Segment's callback from STime_Segment's dctor.
						  //That way, WTime_Segment will learn about STime_Segment's desctruction and return appropriate error codes from that moment on.
	protected:
		ITime_Segment * mSegment;
	public:
		WTime_Segment(ITime_Segment *segment);
		SSignal Get_Signal(const GUID &signal_id);
	};

	class STime_Segment : public std::shared_ptr<ITime_Segment> {
	public:
		SSignal Get_Signal(const GUID &signal_id);
	};

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

	class CTime_Segment final : public virtual ITime_Segment, public virtual refcnt::CReferenced
	{
	private:
		// managed signals; created by calling Get_Signal
		std::map<GUID, glucose::SSignal> mSignals; 

	public:
		// default constructor
		CTime_Segment() = default;
		// disable copying, allow just cloning
		CTime_Segment(const CTime_Segment& b) = delete;
		virtual ~CTime_Segment();

		virtual HRESULT IfaceCalling Get_Signal(const GUID *signal_id, glucose::ISignal **signal) override final;

		// clones this segment into another; calls AddRef (passes ownership to caller)
		STime_Segment Clone();
	};

#pragma warning( pop )

}
