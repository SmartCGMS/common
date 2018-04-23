#pragma once

#include <memory>
#include <string>
#include <vector>

#include "DeviceIface.h"

#include "..\rtl\winapi_mapping.h"

namespace glucose {

	class IFilter_Pipe : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT send(const TDevice_Event *event) = 0;
				//Pipe TAKES ownership of any nested reference-counted I-object so that send-caller is forbidden to call to release the nested objects				
		virtual HRESULT receive(TDevice_Event *event) = 0;
		virtual HRESULT abort() = 0;
	};

	using time_segment_id_container = refcnt::IVector_Container<int64_t>;

	enum class NParameter_Type : size_t {
		ptNull = 0,
		ptWChar_Container,	//IParameter_Container<wchar_t>
		ptSelect_Time_Segment_ID,	//alias for IParameter_Container<int64_t> that selects time segments id
		ptDouble,	
		ptInt64,
		ptBool,
		ptModel_Id,
		ptMetric_Id,
		ptSolver_Id,
		ptModel_Signal_Id,	// signal dependend on model selection
		ptSignal_Id,		// any signal available (measured, calculated)
		ptModel_Bounds,		// three parameter sets in one container - lower bound, default values, higher bound
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
			GUID guid;
			glucose::IModel_Parameter_Vector* parameters;
		};
	};

	class IFilter : public virtual refcnt::IReferenced {
	public:
		// This method is already started within a thread - no need to create another one inside filter
		virtual HRESULT Run(const refcnt::IVector_Container<TFilter_Parameter> *configuration) = 0;
	};

	using TCreate_Filter = HRESULT(IfaceCalling *)(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, glucose::IFilter **filter);
}
