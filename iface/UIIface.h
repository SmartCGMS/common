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

	struct TFilter_Descriptor {
		const GUID id;
		const wchar_t *description;
		const size_t parameters_count;	//can be zero
		const NParameter_Type* parameter_type;
		const wchar_t** ui_parameter_name;		
		const wchar_t** config_parameter_name;		
	};


	using TCreate_Filter = HRESULT(IfaceCalling *)(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, glucose::IFilter **filter);
	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);

}

