#pragma once

#include <memory>
#include <string>
#include <vector>


#include "referencedIface.h"
#include "SensorIface.h"

namespace glucose {

	class IFilter_Pipe : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT send(const TSensor_Event *event) = 0;
		virtual HRESULT receive(TSensor_Event *event) = 0;
	};


	template <typename T>
	class IParameter_Container : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT set(const T *begin, const T *end) = 0;
		virtual HRESULT get(T **begin, T **end) const = 0;
	};

	using wstr_contrainer = IParameter_Container<wchar_t>;

	enum class NParameter_Type : size_t {
		ptNull = 0,
		ptWChar_Container,	//IParameter_Container<wchar_t>		
		ptSelect_Time_Segment_ID,	//alias for IParameter_Container<int64_t> that selects time segments id
		ptDouble,		
	};

	struct TFilter_Parameter {
		//data marshalling to enable inter-operability
		NParameter_Type type;
		wstr_contrainer *config_name;
		union {
			wstr_contrainer* wstr;		//ptWChar_Container
			IParameter_Container<int64_t>* select_time_segment_id;
			double dbl;
		};
	};
	

	class IFilter : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT configure(const TFilter_Parameter *configuration, const size_t count) = 0;
	};

}