/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#include "../iface/DeviceIface.h"
#include "referencedImpl.h"
#include "guid.h"

#include <memory>
#include <map>
#include <vector>

namespace scgms {

	/* shared pointer for model parameter vector; helper class */
	class SModel_Parameter_Vector : public std::shared_ptr<IModel_Parameter_Vector> {
		public:
			/* sets new set of parameters */
			bool set(const std::vector<double> &params);
			/* sets new set of parameters */
			bool set(const SModel_Parameter_Vector &params);
			/* is the model parameter vector empty? */
			bool empty() const;
	};

	class STime_Segment;

	/* shared pointer for signal; helper class */
	class SSignal : public std::shared_ptr<ISignal> {
		public:
			SSignal() {};
			SSignal(STime_Segment segment, const GUID &signal_id);
			SSignal(STime_Segment segment, const GUID& signal_id, const GUID& approx_id);
	};

	/* weak pointer emulation for ISignal interface; convenient for indirect circular dependencies */
	class WSignal {
		protected:
			ISignal *mSignal;

		public:
			WSignal(ISignal *signal);

			/* wrapper for Get_Discrete_Levels call on a wrapped object */
			HRESULT Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const;
			/* wrapper for Get_Discrete_Bounds call on a wrapped object */
			HRESULT Get_Discrete_Bounds(scgms::TBounds* const time_bounds, scgms::TBounds* const level_bounds, size_t *level_count) const;
	};

	/* weak pointer emulation for ITime_Segment interface; convenient for indirect circular dependencies */
	class WTime_Segment {
		protected:
			ITime_Segment *mSegment;

		public:
			WTime_Segment(ITime_Segment *segment);

			/* retrieves the stored signal, if it still exists; nullptr otherwise */
			SSignal Get_Signal(const GUID &signal_id);
	};

	/* shared pointer for ITime_Segment; helper class */
	class STime_Segment : public std::shared_ptr<ITime_Segment> {
		public:
			/* retrieves a signal contained within the time segment */
			SSignal Get_Signal(const GUID &signal_id);
	};

	/* deleter for std::unique_ptr object containing a device event */
	struct UDevice_Event_Deleter {
		void operator()(IDevice_Event *obj_to_release) { 
			if (obj_to_release != nullptr) {
				obj_to_release->Release();
			}
		};
	};

	namespace UDevice_Event_internal {
		/* internal device event major type enumeration */
		enum class NDevice_Event_Major_Type : uint8_t {
			null = 0,
			level,
			info,
			parameters,
			control
		};

		/* retrieves the device event major type according to its interface type */
		NDevice_Event_Major_Type major_type(const scgms::NDevice_Event_Code code);
	}

	/* device event helper class to maintain memory correctly during operation */
	class UDevice_Event : public std::unique_ptr<IDevice_Event, UDevice_Event_Deleter> {
		protected:
			/* raw device event structure; this must be initialized in the constructor exactly once */
			TDevice_Event * mRaw;

		protected:
			/* retrieves device event major type */
			scgms::UDevice_Event_internal::NDevice_Event_Major_Type major_type() const;

		public:
			explicit UDevice_Event(const NDevice_Event_Code code = NDevice_Event_Code::Nothing)  noexcept;
			UDevice_Event(UDevice_Event&& event) noexcept;
			UDevice_Event(IDevice_Event *event);
			void reset(IDevice_Event *event);

			/* clones the device event into a new instance */
			UDevice_Event Clone();

			/* this must be const, because level, parameters and info shared the same data space!
			 * it is 100% fool proof, but programmer should still easily discover the error when overwriting e.g., info with level and then reading info */

			/* retrieves an event code */
			const NDevice_Event_Code& event_code() const;
			/* retrieves device event logical (Lamport) time */
			const int64_t& logical_time() const;

			/* retrieves device GUID */
			GUID& device_id() const;
			/* retrieves contained signal GUID */
			GUID& signal_id() const;
			/* retrieves event rattime */
			double& device_time() const;
			/* retrieves the segment ID */
			uint64_t& segment_id() const;
			/* retrieves the level contained, if it's a level event */
			double& level() const;

			/* stored parameters vector; for parameter events */
			SModel_Parameter_Vector parameters;
			/* stored string; for info events */
			refcnt::Swstr_container info;

			/* is the device event a level event (e.g., contains a level value)? */
			bool is_level_event() const;
			/* is the device event a parameters event (e.g., parameters field is valid)? */
			bool is_parameters_event() const;
			/* is the device event an info event (e.g., info string is valid)? */
			bool is_info_event() const;
			/* is the device event a control event? */
			bool is_control_event() const;
	};

#pragma warning(push)
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

	/* time segment implementation class */
	class CTime_Segment final : public virtual ITime_Segment, public virtual refcnt::CReferenced
	{
		private:
			/* managed signals; created by calling Get_Signal */
			std::map<GUID, scgms::SSignal> mSignals;

		public:
			CTime_Segment() = default;
			CTime_Segment(const CTime_Segment& b) = delete;
			virtual ~CTime_Segment();

			virtual HRESULT IfaceCalling Get_Signal(const GUID *signal_id, scgms::ISignal **signal) override;

			/* clones this segment into another; calls AddRef (passes ownership to caller) */
			STime_Segment Clone();
	};

#pragma warning( pop ) 

}
