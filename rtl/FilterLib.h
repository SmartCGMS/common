#pragma once

#include "../iface/UIIface.h"

#include "referencedImpl.h"
#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;

	void inject_filter_loader(glucose::TGet_Filter_Descriptors get_descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc);
	SFilter_Pipe create_filter_pipe();
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);
	//instantiate a filter and connects it to its input and output

}


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container);
glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
