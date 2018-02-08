#pragma once

#include "DeviceLib.h"

#include "../iface/referencedIface.h"

glucose::SSignal glucose::STime_Segment::Get_Signal(const GUID &signal_id) {
	glucose::SSignal result;
	ISignal *obj = nullptr;
	glucose::ITime_Segment *segment = get();

	if (segment)
		if (segment->Get_Signal(&signal_id, &obj) == S_OK)
			result = refcnt::make_shared_reference<glucose::ISignal>(obj, false);


		return result;
}
	


std::vector<glucose::STime_Segment> glucose::Time_Segments_To_Vector(const glucose::ITime_Segment **segments, const size_t segment_count) {
	std::vector<glucose::STime_Segment> result;

	for (size_t i = 0; i < segment_count; i++)
		result.push_back(refcnt::make_shared_reference_ext<glucose::STime_Segment, glucose::ITime_Segment>(const_cast<glucose::ITime_Segment*>(segments[i]), true));

	return result;
}