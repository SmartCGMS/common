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
		virtual HRESULT get(T **begin, T **end) = 0;
	};

	enum class NParameter_Type : size_t {
		ptNull = 0,
		ptWChar_Container,	//IParameter_Container<wchar_t>
		ptInt64_Container,  //IParameter_Container<int64_t>
		ptDouble
	};

	struct TFilter_Parameter {
		//data marshalling to enable inter-operability
		NParameter_Type type;
		
		union {
			IParameter_Container<wchar_t>* wstr;		//ptWChar_Container
			IParameter_Container<int64_t>* vec_int64;
			double dbl;
		};
	};
	

	class IFilter : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT configure(const TFilter_Parameter *configuration, const size_t count) = 0;
	};

}