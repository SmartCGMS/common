#pragma once

#include "DeviceLib.h"

#include "../iface/referencedIface.h"

glucose::SSignal glucose::WTime_Segment::Get_Signal(const GUID &signal_id) {
	auto shared = lock();
	if (shared) {		
		glucose::STime_Segment converted = refcnt::make_shared_reference_ext<glucose::STime_Segment, glucose::ITime_Segment>(shared.get(), true);
		return converted.Get_Signal(signal_id);
		
	}	else return glucose::SSignal{};
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
	

