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

	

	class IFilter_Factory : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT description(wchar_t **result) const = 0;
			//returns filter description

		virtual HRESULT create_filter(IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter) = 0;
		//instiantiate a filter and connects it to its input and output
	};


}