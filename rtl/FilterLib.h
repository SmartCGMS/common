#pragma once

#include "../iface/FilterIface.h"

#include <vector>

namespace glucose {
	#ifdef _WIN32
		extern "C" __declspec(dllimport) std::vector<SFilter_Factory> __fastcall  get_filter_factories();
		extern "C"__declspec(dllimport) SFilter_Pipe __fastcall create_filter_pipe();
	#endif
}