#pragma once

#include "../iface/UIIface.h"

#include "manufactory.h"
#include "referencedImpl.h"
#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptors_by_id(const GUID &id, TFilter_Descriptor &desc);
	SFilter_Pipe create_filter_pipe();
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);
	//instantiate a filter and connects it to its input and output


	namespace internal {

		#pragma warning( push )
		#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

		template <typename T>
		class CParameter_Container : public virtual IParameter_Container<T>, public virtual refcnt::CReferenced, public std::vector<T> {
		public:
			virtual ~CParameter_Container() {};

			virtual HRESULT set(const T *begin, const T *end) final {
				clear();
				std::copy(begin, end, std::back_inserter(*this));
				return S_OK;
			}

			virtual HRESULT get(T **begin, T **end) const final {
				*begin = const_cast<T*>(data());
				*end = const_cast<T*>(data()) +size();
				return S_OK;
			}
		};

		#pragma warning( pop ) 
	}


	template <typename T>
	std::shared_ptr<IParameter_Container<T>> Create_Parameter_Container(const T *begin, const T *end) {
		IParameter_Container<T> *obj = nullptr;
		if (Manufacture_Object<internal::CParameter_Container<T>, IParameter_Container<T>>(&obj) == S_OK)
			obj->set(begin, end);
		return refcnt::make_shared_reference_ext <std::shared_ptr<IParameter_Container<T>>, IParameter_Container<T>>(obj, false);	
	}

}

std::wstring WChar_Container_To_WString(glucose::wstr_contrainer *container);
glucose::wstr_contrainer* WString_To_WChar_Container(const wchar_t* str);
bool WChar_Container_Equals_WString(glucose::wstr_contrainer *container, const wchar_t* str);


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);

