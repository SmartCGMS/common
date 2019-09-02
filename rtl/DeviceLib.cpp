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
 * Univerzitni 8
 * 301 00, Pilsen
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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include "../iface/referencedIface.h"

#include "DeviceLib.h"
#include "rattime.h"
#include "FactoryLib.h"

namespace imported {
	glucose::TCreate_Signal create_signal = factory::resolve_symbol<glucose::TCreate_Signal>("create_signal");
	glucose::TCreate_Device_Event create_device_event = factory::resolve_symbol<glucose::TCreate_Device_Event>("create_device_event");
}


bool glucose::SModel_Parameter_Vector::set(const std::vector<double> &params) {
	double *data_ptr = const_cast<double*>(params.data());
	if (!operator bool()) {
		glucose::IModel_Parameter_Vector *new_vector = refcnt::Create_Container<double>(data_ptr, data_ptr + params.size());
		reset(new_vector, [](glucose::IModel_Parameter_Vector* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
		return operator bool();
	} else
		return get()->set(data_ptr, data_ptr + params.size()) == S_OK;
}


bool glucose::SModel_Parameter_Vector::set(const glucose::SModel_Parameter_Vector &params) {
	if (!params) return false;

	double *begin, *end;
	if (params->get(&begin, &end) != S_OK) return false;

	if (!operator bool()) {
		glucose::IModel_Parameter_Vector *new_vector = refcnt::Create_Container<double>(begin, end);
		reset(new_vector, [](glucose::IModel_Parameter_Vector* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
		return operator bool();
	} else
		return get()->set(begin, end) == S_OK;
}

bool glucose::SModel_Parameter_Vector::empty() const {
	if (!operator bool()) return true;
	return get()->empty() == S_OK;
}

glucose::SSignal::SSignal(glucose::STime_Segment segment, const GUID &signal_id) {
	glucose::ISignal *signal;
	if (imported::create_signal(&signal_id, segment.get(), &signal) == S_OK) {
		reset(signal, [](glucose::ISignal* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}
}

glucose::WSignal::WSignal(ISignal *signal) : mSignal(signal) {};

HRESULT glucose::WSignal::Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const {
	return mSignal ? mSignal->Get_Discrete_Levels(times, levels, count, filled) : E_FAIL;
}

HRESULT glucose::WSignal::Get_Discrete_Bounds(glucose::TBounds* const time_bounds, glucose::TBounds* const level_bounds, size_t *level_count) const {
	return mSignal ? mSignal->Get_Discrete_Bounds(time_bounds, level_bounds, level_count) : E_FAIL;
}


glucose::WTime_Segment::WTime_Segment(ITime_Segment *segment) : mSegment(segment) {

}

glucose::SSignal glucose::WTime_Segment::Get_Signal(const GUID &signal_id) {
	glucose::SSignal result;
	ISignal *obj = nullptr;

	if (mSegment)
		if (mSegment->Get_Signal(&signal_id, &obj) == S_OK)
			result = refcnt::make_shared_reference_ext<glucose::SSignal, glucose::ISignal>(obj, false);

	return result;
}

glucose::SSignal glucose::STime_Segment::Get_Signal(const GUID &signal_id) {
	glucose::SSignal result;
	ISignal *obj = nullptr;
	glucose::ITime_Segment *segment = get();

	if (segment)
		if (segment->Get_Signal(&signal_id, &obj) == S_OK)
			result = refcnt::make_shared_reference_ext<glucose::SSignal, glucose::ISignal>(obj, false);

	return result;
}

glucose::TDevice_Event* Get_Raw_Event(glucose::IDevice_Event *event) {
	glucose::TDevice_Event* result;
	if ((event == nullptr) || (event->Raw(&result) != S_OK)) result = nullptr;

	return result;
}

glucose::IDevice_Event* Create_Event(const glucose::NDevice_Event_Code code) {
	glucose::IDevice_Event *result;
	if (imported::create_device_event(code, &result) != S_OK) result = nullptr;
	return result;
}

glucose::UDevice_Event::UDevice_Event(const glucose::NDevice_Event_Code code) noexcept : UDevice_Event(Create_Event(code)) {
}

glucose::UDevice_Event::UDevice_Event(glucose::UDevice_Event&& event) : mRaw(event.mRaw) {
	event.release();
}

glucose::UDevice_Event::UDevice_Event(IDevice_Event *event) : std::unique_ptr<IDevice_Event, UDevice_Event_Deleter>(event), mRaw(Get_Raw_Event(event)){
	switch (major_type()) {
		case glucose::UDevice_Event_internal::NDevice_Event_Major_Type::info:
			if (mRaw->info) info = refcnt::make_shared_reference_ext<refcnt::Swstr_container, refcnt::wstr_container>(mRaw->info, true);
			break;

		case glucose::UDevice_Event_internal::NDevice_Event_Major_Type::parameters:
			if (mRaw->parameters) parameters = refcnt::make_shared_reference_ext<glucose::SModel_Parameter_Vector, glucose::IModel_Parameter_Vector>(mRaw->parameters, true);
			break;

		default:
			break;
	}
}


glucose::UDevice_Event_internal::NDevice_Event_Major_Type glucose::UDevice_Event::major_type() const {
	if (mRaw == nullptr) return glucose::UDevice_Event_internal::NDevice_Event_Major_Type::null;

	switch (mRaw->event_code) {
		case glucose::NDevice_Event_Code::Level:
		case glucose::NDevice_Event_Code::Masked_Level:		return glucose::UDevice_Event_internal::NDevice_Event_Major_Type::level;

		case glucose::NDevice_Event_Code::Parameters:
		case glucose::NDevice_Event_Code::Parameters_Hint:	return glucose::UDevice_Event_internal::NDevice_Event_Major_Type::parameters;

		case glucose::NDevice_Event_Code::Information:
		case glucose::NDevice_Event_Code::Warning:
		case glucose::NDevice_Event_Code::Error:			return glucose::UDevice_Event_internal::NDevice_Event_Major_Type::info;

		default:	break;
	}

	return glucose::UDevice_Event_internal::NDevice_Event_Major_Type::null;
}

void glucose::UDevice_Event::reset(IDevice_Event *event) {
	if (operator bool()) {
		auto deleter = get_deleter();
		auto obj = get();
		release();
		deleter(obj);
	}

	if (event) {
		if (event->Raw(&mRaw) == S_OK) event->Release();	//do not forget that we're moving the object!
	}
	
}

const glucose::NDevice_Event_Code& glucose::UDevice_Event::event_code() const {
	return mRaw->event_code;
}

const int64_t& glucose::UDevice_Event::logical_time() const {
	return mRaw->logical_time;
}

GUID& glucose::UDevice_Event::device_id() const {
	return mRaw->device_id;
}

GUID& glucose::UDevice_Event::signal_id() const {
	return mRaw->signal_id;
}

double& glucose::UDevice_Event::device_time() const {
	return mRaw->device_time;
}
	
uint64_t& glucose::UDevice_Event::segment_id() const {
	return mRaw->segment_id;
}

double& glucose::UDevice_Event::level() const {
	return mRaw->level;
}


bool glucose::UDevice_Event::is_level_event() const {
	return major_type() == glucose::UDevice_Event_internal::NDevice_Event_Major_Type::level;
}

bool glucose::UDevice_Event::is_parameters_event() const {
	return major_type() == glucose::UDevice_Event_internal::NDevice_Event_Major_Type::parameters;
}

bool glucose::UDevice_Event::is_info_event() const {
	return major_type() == glucose::UDevice_Event_internal::NDevice_Event_Major_Type::info;
}


glucose::CTime_Segment::~CTime_Segment()
{
	//
}

HRESULT IfaceCalling glucose::CTime_Segment::Get_Signal(const GUID *signal_id, glucose::ISignal **signal)
{
	auto itr = mSignals.find(*signal_id);
	if (itr != mSignals.end())
	{
		*signal = (*itr).second.get();
		(*signal)->AddRef();
		return S_OK;
	}

	// prefer calculated signal, fall back to measured signal
	if (imported::create_signal(signal_id, this, signal) != S_OK) {
		return E_NOTIMPL;
	}
	mSignals[*signal_id] = refcnt::make_shared_reference_ext<glucose::SSignal, glucose::ISignal>(*signal, true);  // true due to creating "clone" of pointer with custom reference counter

	return S_OK;
}

glucose::STime_Segment glucose::CTime_Segment::Clone()
{
	// manufacture new segment
	glucose::CTime_Segment* cloned;
	if (Manufacture_Object<glucose::CTime_Segment>(&cloned) != S_OK)
		return {};

	size_t count;
	glucose::ISignal* target;

	std::vector<double> tmpTimes, tmpLevels;

	// clone each signal
	for (auto const& signal : mSignals)
	{
		// create signal in cloned object
		if (cloned->Get_Signal(&signal.first, &target) != S_OK)
			continue;

		// retrieve discrete bounds (this will fail for calculated signals, which we are not fancy copying)
		if (signal.second->Get_Discrete_Bounds(nullptr, nullptr, &count) != S_OK)
			continue;

		if (count == 0)
			continue;

		// resize to fit
		tmpTimes.resize(count);
		tmpLevels.resize(count);

		size_t filled = count;

		// retrieve all values
		if (signal.second->Get_Discrete_Levels(tmpTimes.data(), tmpLevels.data(), count, &filled) != S_OK)
			continue;

		// update cloned signal
		target->Add_Levels(tmpTimes.data(), tmpLevels.data(), filled);
	}

	return refcnt::make_shared_reference_ext<glucose::STime_Segment, glucose::ITime_Segment>(cloned, false);
}
