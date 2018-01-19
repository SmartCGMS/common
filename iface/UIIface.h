#pragma once

#include "FilterIface.h"

#ifdef _WIN32
	#include <guiddef.h>
#else
	struct GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};
#endif

namespace glucose {

	using TConfigure_Filter = HRESULT(IfaceCalling *)(std::string &configuration);
		//accept configuration and possiblyc changes it
		//	S_OK configuration was changed
		//	S_FALSE configuration was not changed
		//	other code means an error

	struct TFilter_Descriptor {
		const GUID id;
		const wchar_t *description;
		TConfigure_Filter configurator;	//can be nullptr
	};


	using TCreate_Filter = HRESULT(IfaceCalling *)(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, glucose::IFilter **filter);
	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);
	
}

