#pragma once

#include "../iface/UIIface.h"

#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;
	

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	SFilter_Pipe create_filter_pipe();
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);
	//instiantiate a filter and connects it to its input and output
}