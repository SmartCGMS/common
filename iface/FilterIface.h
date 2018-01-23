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


	enum class NParameter_Type : size_t {
		ptInvalid = 0,
		ptString
	};

	struct TFilter_Parameter {
		//data marshalling to enable inter-operability
		NParameter_Type type;
		
		union {
			const char* c_str;
		};
	};
	

	class IFilter : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT configure(const TFilter_Parameter *configuration, const size_t count) = 0;
	};

}