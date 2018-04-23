#pragma once

#include "DeviceLib.h"

#include "../iface/referencedIface.h"

namespace imported {
	#ifdef _WIN32
		extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_calculated_signal(const GUID *calc_id, glucose::ITime_Segment *segment, glucose::ISignal **signal);
		extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_measured_signal(const GUID *calc_id, glucose::ITime_Segment *segment, glucose::ISignal **signal);
	#endif
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
	if (imported::create_calculated_signal(signal_id, this, signal) != S_OK)
	{
		if (imported::create_measured_signal(signal_id, this, signal) != S_OK)
			return E_NOTIMPL;
	}
	mSignals[*signal_id] =  refcnt::make_shared_reference_ext<glucose::SSignal, glucose::ISignal>(*signal, true);  // true due to creating "clone" of pointer with custom reference counter

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
		if (signal.second->Get_Discrete_Bounds(nullptr, &count) != S_OK)
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

	return refcnt::make_shared_reference_ext<glucose::STime_Segment, glucose::ITime_Segment>(cloned, true);
}
