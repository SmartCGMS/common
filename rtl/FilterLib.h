#pragma once

#include "DeviceLib.h"
#include "../iface/UIIface.h"

#include "referencedImpl.h"
#include <vector>

namespace glucose {

	
	class SFilter_Pipe : public std::shared_ptr<IFilter_Pipe> {
	public:
		SFilter_Pipe(glucose::IFilter_Pipe *pipe);
		SFilter_Pipe();
		bool Send(UDevice_Event &event);
		UDevice_Event Receive();
	};


	using SFilter = std::shared_ptr<IFilter>;

	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);
	
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);

	class SFilter_Parameters : public std::shared_ptr<glucose::IFilter_Configuration> {
	protected:
		TFilter_Parameter* Resolve_Parameter(const wchar_t* name);
	public:
		std::wstring Read_String(const wchar_t* name);
		int64_t Read_Int(const wchar_t* name);
		std::vector<int64_t> Read_Int_Array(const wchar_t* name);
		GUID Read_GUID(const wchar_t* name);
		bool Read_Bool(const wchar_t* name);
	};


	/* TODO delete, once entire project compiles with SFilter_Pipe and SDevice_Event only
	// increases reference counter on nested reference-counter I-objects due to passing container through pipes;
	// this actually adds the whole filter chain as additional "owner", but since we can't use shared_ptrs in the whole chain,
	// we need to manually add reference and release it later
	// this is needed for shared_ptrs (made from IReferenced) stored locally in filter which created them, because when filter
	// releases this local reference, the reference counter may drop to 0 earlier than expected, and may lead to crash
	void AddRef_Event(TDevice_Event &event);

	//calls release on any nested reference-counted I-objects inside the event
	void Release_Event(TDevice_Event &event);
	*/

	void Release_Filter_Parameter(TFilter_Parameter &parameter);


	class SError_Filter_Inspection : public std::shared_ptr<IError_Filter_Inspection> {
	public:
		SError_Filter_Inspection() noexcept {};
		SError_Filter_Inspection(SFilter &error_filter);
	};

	class SDrawing_Filter_Inspection : public std::shared_ptr<IDrawing_Filter_Inspection> {
	public:
		SDrawing_Filter_Inspection() noexcept {};
		SDrawing_Filter_Inspection(SFilter &drawing_filter);
	};
}


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container);
glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
