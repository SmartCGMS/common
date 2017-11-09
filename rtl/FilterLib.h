#pragma once

#include "../iface/FilterIface.h"

#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;
	
	class SFilter_Factory : public std::shared_ptr<IFilter_Factory> {
	public:
		wchar_t* description() const;		

		SFilter create_filter(SFilter_Pipe &input, SFilter_Pipe &output);
			//instiantiate a filter and connects it to its input and output
	};

	std::vector<SFilter_Factory> get_filter_factories();
	SFilter_Pipe create_filter_pipe();
	
}