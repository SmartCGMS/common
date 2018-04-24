#pragma once

#include "../iface/UIIface.h"

#include "referencedImpl.h"
#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;

	void register_additional_filter(glucose::TGet_Filter_Descriptors get_descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc);
	SFilter_Pipe create_filter_pipe();
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);
	//instantiate a filter and connects it to its input and output

	void Release_Event(TDevice_Event &event);
		//calls release on any nested reference-counted I-objects inside the event

	class SError_Filter_Inspection : public std::shared_ptr<IError_Filter_Inspection> {
	public:
		SError_Filter_Inspection(SFilter &error_filter);
	};

	class SDrawing_Filter_Inspection : public std::shared_ptr<IDrawing_Filter_Inspection> {
	public:
		SDrawing_Filter_Inspection(SFilter &drawing_filter);
	};
}


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container);
glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
