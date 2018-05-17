#pragma once

#include "../iface/referencedIface.h"
#include "manufactory.h"

#include <atomic>
#include <string>
#include <vector>

namespace refcnt {

	class CReferenced : public virtual IReferenced {
	protected:
		std::atomic<ULONG> mCounter;
		template <typename I>
		bool Internal_Query_Interface(const GUID &I_id, const GUID &riid, void **ppvObj) {
			const bool result = (I_id == riid);
			if (result) {
				*ppvObj = dynamic_cast<I*>(this);
				AddRef();
			}

			return result;
		}
	public:
		CReferenced() noexcept : mCounter(0) {};
		virtual ~CReferenced() {};

		virtual HRESULT IfaceCalling QueryInterface(const GUID*  riid, void ** ppvObj);
		virtual ULONG IfaceCalling AddRef();
		virtual ULONG IfaceCalling Release();
	};


	class CNotReferenced : public CReferenced {
	public:
		virtual ULONG IfaceCalling AddRef();
		virtual ULONG IfaceCalling Release();
	};



	namespace internal {

		#pragma warning( push )
		#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

		template <typename T>
		class CVector_Container : public virtual IVector_Container<T>, public virtual CReferenced, public std::vector<T> {
		public:
			virtual ~CVector_Container() {};

			virtual HRESULT set(const T *begin, const T *end) override final {
				std::vector<T>::clear();
				if (begin != nullptr)
					std::copy(begin, end, std::back_inserter(*this));
				return S_OK;
			}

			virtual HRESULT get(T **begin, T **end) const override final {
				if (!std::vector<T>::empty()) {
					*begin = const_cast<T*>(std::vector<T>::data());
					*end = const_cast<T*>(std::vector<T>::data()) + std::vector<T>::size();
					return S_OK;
				} else {
					*begin = *end = nullptr;
					return S_FALSE;
				}				
			}

			virtual HRESULT empty() const override final {
				return std::vector<T>::empty() ? S_OK : S_FALSE;
			}
		};

		#pragma warning( pop ) 
	}

	template <typename T>
	IVector_Container<T>* Create_Container(const T *begin, const T *end) {
		IVector_Container<T> *obj = nullptr;
		if (Manufacture_Object<internal::CVector_Container<T>, IVector_Container<T>>(&obj) == S_OK)
			obj->set(begin, end);
		return obj;		
	}


	template <typename T>
	std::shared_ptr<IVector_Container<T>> Create_Container_shared(const T *begin, const T *end) {
		IVector_Container<T> *obj = Create_Container<T>(begin, end);
		return refcnt::make_shared_reference_ext <std::shared_ptr<IVector_Container<T>>, IVector_Container<T>>(obj, false);
	}

	template <typename T>
	std::shared_ptr<IVector_Container<T>> Copy_Container(std::shared_ptr<IVector_Container<T>> src) {
		std::shared_ptr<IVector_Container<T>> result;
		// copy parameter hint to internal vector
		T *begin, *end;
		if (src->get(&begin, &end) == S_OK)
			result = Create_Container_shared<T>(begin, end);

		return result;
	}	


	template <typename T, typename C = std::vector<T>>
	C Container_To_Vector(IVector_Container<T> *container) {
		T *begin, *end;
		if (container->get(&begin, &end) == S_OK) {
			return C{ begin, end };
		}
		else
			return C{};
	}

	std::string Char_Container_To_String(str_container *container);
	
	std::wstring WChar_Container_To_WString(wstr_container *container);
	wstr_container* WString_To_WChar_Container(const wchar_t* str);
	std::shared_ptr<wstr_container> WString_To_WChar_Container_shared(const wchar_t* str);
	bool WChar_Container_Equals_WString(wstr_container *container, const wchar_t* str, size_t offset = 0, size_t maxCount = (size_t)-1);



	template <typename S, typename I>
	std::vector<S> Referenced_To_Vector(I **items, const size_t count) {
		std::vector<S> result;

		for (size_t i = 0; i < count; i++)
			result.push_back(refcnt::make_shared_reference_ext<S, I>(const_cast<I*>(items[i]), true));

		return result;
	}


	class Swstr_container : public std::shared_ptr<refcnt::wstr_container> {
	public:
		void set(const wchar_t *str);
		bool operator==(const wchar_t *other) const;
	};
	
}