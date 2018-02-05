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
	} ;

	inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
	{
		return !memcmp(&rguid1, &rguid2, sizeof(GUID));
	}

	inline bool operator==(const GUID& guidOne, const GUID& guidOther)
	{
		return !!IsEqualGUID(guidOne, guidOther);
	}
#endif

namespace glucose {

	struct TFilter_Descriptor {
		GUID id;
		const wchar_t *description;
		size_t parameters_count;	//can be zero
		const NParameter_Type* parameter_type;
		const wchar_t** ui_parameter_name;
		const wchar_t** config_parameter_name;
	};


	using TCreate_Filter = HRESULT(IfaceCalling *)(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, glucose::IFilter **filter);
	using TGet_Filter_Descriptors = HRESULT(IfaceCalling*)(TFilter_Descriptor **begin, TFilter_Descriptor **end);

}

