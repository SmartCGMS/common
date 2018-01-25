#pragma once

#include "../iface/UIIface.h"

#include "manufactory.h"
#include "referencedImpl.h"
#include <vector>

namespace glucose {

	using SFilter_Pipe = std::shared_ptr<IFilter_Pipe>;
	using SFilter = std::shared_ptr<IFilter>;

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	SFilter_Pipe create_filter_pipe();
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);
	//instiantiate a filter and connects it to its input and output


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

			virtual HRESULT get(T **begin, T **end) final {
				*begin = data();
				*end = data()+size();
				return S_OK;
			}
		};

		#pragma warning( pop ) 
	}


	template <typename T>
	std::shared_ptr<IParameter_Container<T>> Create_Parameter_Container(const T *begin, const T *end) {
		IParameter_Container<T> *obj = nullptr;
		Manufacture_Object<internal::CParameter_Container<T>, IParameter_Container<T>>(&obj);
		return refcnt::make_shared_reference_ext <std::shared_ptr<IParameter_Container<T>>, IParameter_Container<T>>(obj, false);	
	}
}