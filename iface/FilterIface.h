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

	

	class IFilter : public virtual refcnt::IReferenced {
	public:
	};

}