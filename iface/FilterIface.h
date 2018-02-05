#pragma once

#include <memory>
#include <string>
#include <vector>


#include "DeviceIface.h"

namespace glucose {

	class IFilter_Pipe : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT send(const TDevice_Event *event) = 0;
		virtual HRESULT receive(TDevice_Event *event) = 0;
	};


	
	using time_segment_id_container = refcnt::IVector_Container<int64_t>;

	enum class NParameter_Type : size_t {
		ptNull = 0,
		ptWChar_Container,	//IParameter_Container<wchar_t>		
		ptSelect_Time_Segment_ID,	//alias for IParameter_Container<int64_t> that selects time segments id
		ptDouble,	
		ptInt64,
		ptBool
	};

	struct TFilter_Parameter {
		//data marshalling to enable inter-operability
		NParameter_Type type;
		refcnt::wstr_container *config_name;
		union {
			refcnt::wstr_container* wstr;		//ptWChar_Container
			time_segment_id_container* select_time_segment_id;
			double dbl;
			int64_t int64;
			uint8_t boolean;
		};
	};
	

	class IFilter : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT configure(const TFilter_Parameter *configuration, const size_t count) = 0;
	};

}