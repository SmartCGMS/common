#pragma once

#ifdef _WIN32
	#include <guiddef.h>
#else
	struct GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};

	inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
	{
		return !memcmp(&rguid1, &rguid2, sizeof(GUID));
	}

	inline bool operator==(const GUID& guidOne, const GUID& guidOther)
	{
		return !!IsEqualGUID(guidOne, guidOther);
	}
#endif