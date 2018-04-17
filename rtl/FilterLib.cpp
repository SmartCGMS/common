#include "FilterLib.h"

#include <sstream>
#include "manufactory.h"

namespace glucose {


	namespace imported {

		#ifdef _WIN32
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling get_filter_descriptors(TFilter_Descriptor **begin, TFilter_Descriptor **end);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter_pipe(IFilter_Pipe **pipe);
			extern "C" __declspec(dllimport)  HRESULT IfaceCalling create_filter(const GUID *id, IFilter_Pipe *input, IFilter_Pipe *output, IFilter **filter);
		#endif
	}

	namespace injected
	{
		std::vector<glucose::TGet_Filter_Descriptors> get_filter_descriptors;
		std::vector<glucose::TCreate_Filter> create_filter;
	}

	void register_additional_filter(glucose::TGet_Filter_Descriptors get_descriptors, glucose::TCreate_Filter create_filter)
	{
		injected::get_filter_descriptors.push_back(get_descriptors);
		injected::create_filter.push_back(create_filter);
	}

	std::vector<TFilter_Descriptor> get_filter_descriptors() {
		std::vector<TFilter_Descriptor> result;
		TFilter_Descriptor *desc_begin, *desc_end;

		if (imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK) {
			std::copy(desc_begin, desc_end, std::back_inserter(result));
		}

		for (auto getter : injected::get_filter_descriptors)
		{
			if (getter(&desc_begin, &desc_end) == S_OK) {
				std::copy(desc_begin, desc_end, std::back_inserter(result));
			}
		}

		return result;
	}

	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc) {
		TFilter_Descriptor *desc_begin, *desc_end;

		bool result = imported::get_filter_descriptors(&desc_begin, &desc_end) == S_OK;
		if (result) {
			result = false;	//we have to find the filter yet
			for (auto iter = desc_begin; iter != desc_end; iter++)
				if (iter->id == id) {
					//desc = *iter;							assign const won't work with const members and custom operator= will result into undefined behavior as it has const members (so it does not have to be const itself)
					memcpy(&desc, iter, sizeof(decltype(desc)));	//=> memcpy https://stackoverflow.com/questions/9218454/struct-with-const-member
					result = true;
					break;
				}
		}

		if (!result)
		{
			for (auto getter : injected::get_filter_descriptors)
			{
				if (getter(&desc_begin, &desc_end) == S_OK)
				{
					for (auto iter = desc_begin; iter != desc_end; iter++)
					{
						if (iter->id == id) {
							// see note above (memcpy)
							memcpy(&desc, iter, sizeof(decltype(desc)));
							result = true;
							break;
						}
					}
				}
			}
		}

		return result;
	}


	SFilter_Pipe create_filter_pipe() {
		SFilter_Pipe result;
		IFilter_Pipe *pipe;
		if (imported::create_filter_pipe(&pipe) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter_Pipe, IFilter_Pipe>(pipe, false);

		return result;
	}

	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output) {
		SFilter result;
		IFilter *filter;

		if (imported::create_filter(&id, input.get(), output.get(), &filter) == S_OK)
			result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);
		else {
			for (auto builder : injected::create_filter)
			{
				if (builder(&id, input.get(), output.get(), &filter) == S_OK)
				{
					result = refcnt::make_shared_reference_ext<SFilter, IFilter>(filter, false);
					break;
				}
			}
		}

		return result;
	}

	


}



std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container) {
	int64_t *begin, *end;
	if (container->get(&begin, &end) == S_OK) {
		std::wstring result;
		for (auto iter = begin; iter != end; iter++) {
			if (result.size() > 0) result.append(L" ");

			result.append(std::to_wstring(*iter));
		}
		return result;
	}
	else
		return std::wstring{};
}

glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str) {
	glucose::time_segment_id_container *obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<int64_t>, glucose::time_segment_id_container>(&obj) == S_OK) {
		std::vector<int64_t> ids;

		std::wstringstream value_str(str);
		int64_t value;
		while (value_str >> value)
			ids.push_back(value);
		
		obj->set(ids.data(), ids.data()+ ids.size());
		return obj;
	}
	else
		return nullptr;
}

std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container)
{
	std::wstring result;

	double *begin, *end;
	if (container->get(&begin, &end) == S_OK)
	{
		for (auto iter = begin; iter != end; iter++)
		{
			if (result.size() > 0)
				result += L" ";

			result += std::to_wstring(*iter);
		}
	}

	return result;
}

glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str)
{
	glucose::IModel_Parameter_Vector *obj = nullptr;
	if (Manufacture_Object<refcnt::internal::CVector_Container<double>, glucose::IModel_Parameter_Vector>(&obj) == S_OK)
	{
		std::vector<double> params;

		std::wstringstream value_str(str);
		double value;
		while (value_str >> value)
			params.push_back(value);

		obj->set(params.data(), params.data() + params.size());
		return obj;
	}
	else
		return nullptr;
}
