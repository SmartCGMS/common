#pragma once

#include "../iface/DeviceIface.h"
#include "referencedImpl.h"
#include "guid.h"

#include <memory>
#include <map>
#include <vector>

namespace glucose {

	class SModel_Parameter_Vector : public std::shared_ptr<IModel_Parameter_Vector> {
	public:
		bool set(const std::vector<double> &params);
		bool set(const SModel_Parameter_Vector &params);
		bool empty() const;
	};

	class STime_Segment;

	class SSignal : public std::shared_ptr<ISignal> {
	public:
		SSignal() {};	//just an empty object
		SSignal(STime_Segment segment, const GUID &signal_id);
	};

	class WSignal {
	protected:
		ISignal * mSignal;
	public:
		WSignal(ISignal *signal);

		HRESULT Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const;
		HRESULT Get_Discrete_Bounds(TBounds *bounds, size_t *level_count) const;
	};

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


	//because unique_ptr::reset does not let us to specify deleter lambda, let's do it this way
	struct UDevice_Event_Deleter {
		void operator()(IDevice_Event *obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); };
	};

	namespace UDevice_Event_internal {
		enum class NDevice_Event_Major_Type {
			null,
			level,
			info,
			parameters
		};
	}

	class UDevice_Event : public std::unique_ptr<IDevice_Event, UDevice_Event_Deleter> {
	protected:
		TDevice_Event * mRaw;		//mRaw must be initialized in the constructor exactly once
									//therefore, the implementation defines two helper functions,
									//which returns pointers only
		UDevice_Event_internal::NDevice_Event_Major_Type major_type() const;
	public:
		explicit UDevice_Event(const NDevice_Event_Code code = NDevice_Event_Code::Nothing);
		UDevice_Event(IDevice_Event *event);
		void reset(IDevice_Event *event) = delete;		//reset would break references tight to mRaw, therefore it is disallowed

														//this must be const, because level, parameters and info shared the same data space!!!
														//it is 100% fool proof, but programmer should still easily discover the error when overwriting e.g., info with level and then reading info
		const NDevice_Event_Code &event_code = mRaw->event_code;
		const int64_t &logical_time = mRaw->logical_time;

		GUID &device_id = mRaw->device_id;
		GUID &signal_id = mRaw->signal_id;
		double &device_time = mRaw->device_time;
		uint64_t &segment_id = mRaw->segment_id;
		double &level = mRaw->level;


		SModel_Parameter_Vector parameters;
		refcnt::Swstr_container info;

		bool is_level_event() const;
		bool is_parameters_event() const;
		bool is_info_event() const;
	};


#pragma warning(push)
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

		virtual HRESULT IfaceCalling Get_Signal(const GUID *signal_id, glucose::ISignal **signal) override;

		// clones this segment into another; calls AddRef (passes ownership to caller)
		STime_Segment Clone();
	};

#pragma warning( pop ) 

}
