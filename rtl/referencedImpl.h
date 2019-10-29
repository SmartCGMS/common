/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8
 * 301 00, Pilsen
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

#include "../iface/referencedIface.h"
#include "manufactory.h"
#include "AlignmentAllocator.h"

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


	template <typename I>
	class SReferenced : public std::shared_ptr<I> {
	public:
		SReferenced() : std::shared_ptr<I>() {}

		SReferenced(I *obj) {			
			if (obj) obj->AddRef();
			std::shared_ptr<I>::reset(obj, [](I* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
		}

		virtual ~SReferenced() {}
	};


	namespace internal {

		#pragma warning( push )
		#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

		template <typename T>
		class TAligned_Vector : public std::vector<T, AlignmentAllocator<T>>, public CAligned<> {
		};

		template <typename T>
		class CVector_Container : public virtual IVector_Container<T>, public virtual CReferenced, public TAligned_Vector<T> {
		private:
			template <typename D = typename std::remove_pointer<T>::type>
			typename std::enable_if<!std::is_base_of<refcnt::IReferenced, D>::value && !std::is_base_of<refcnt::IUnique_Reference, D>::value, void>::type
			Add_Content(T *begin, T *end) {
				std::copy(begin, end, std::back_inserter(*this));
			};

			template <typename D = typename std::remove_pointer<T>::type>
			typename std::enable_if<std::is_base_of<refcnt::IReferenced, D>::value, void>::type
			Add_Content(T *begin, T *end) {
				for (T* iter = begin; iter != end; iter++) {
					T real_ptr = *iter;
					TAligned_Vector<T>::push_back(real_ptr);
					real_ptr->AddRef();
				}
			}

			template <typename D = typename std::remove_pointer<T>::type>
			typename std::enable_if<std::is_base_of<refcnt::IUnique_Reference, D>::value, void>::type
				Add_Content(T *begin, T *end) {
				for (T* iter = begin; iter != end; iter++) {
					T real_ptr = *iter;
					TAligned_Vector<T>::push_back(std::move(real_ptr));
					*iter = nullptr;	//now, we own the unique reference and therefore we clean the caller's data not to ever use the reference again
				}
			}


			template <typename D = typename std::remove_pointer<T>::type>
			typename std::enable_if<!std::is_base_of<refcnt::IReferenced, D>::value && !std::is_base_of<refcnt::IUnique_Reference, D>::value, void>::type
			Release_Item(T &item) {};

			template <typename D = typename std::remove_pointer<T>::type>
			typename std::enable_if<std::is_base_of<refcnt::IReferenced, D>::value || std::is_base_of<refcnt::IUnique_Reference, D>::value, void>::type
			Release_Item(T &item) {				
				item->Release();	//as we own both kind of references, we release them
			}

			void Release_Content() {
				for (T &item : *this)
					Release_Item(item);	//as we own both kind of references, we release them
			}
			

		public:
			virtual ~CVector_Container() { Release_Content();  };

			virtual HRESULT set(T *begin, T *end) override final {
				Release_Content();
				TAligned_Vector<T>::clear();
				return add(begin, end);
			}

			virtual HRESULT add(T *begin, T *end) override final {
				if (begin != nullptr)
					Add_Content(begin, end);
				return S_OK;
			}

			virtual HRESULT get(T **begin, T **end) const override final {
				if (!TAligned_Vector<T>::empty()) {
					*begin = const_cast<T*>(TAligned_Vector<T>::data());
					*end = const_cast<T*>(TAligned_Vector<T>::data()) + TAligned_Vector<T>::size();
					return S_OK;
				} else {
					*begin = *end = nullptr;
					return S_FALSE;
				}
			}
		
			virtual HRESULT pop(T* value) override final {
				if (TAligned_Vector<T>::empty()) return S_FALSE;

				*value = TAligned_Vector<T>::back();
				TAligned_Vector<T>::pop_back();

				//there is no reference add_refing/releasing needed, because we are moving from the vector out
				return S_OK;
			}

			virtual HRESULT remove(const size_t index) {
				if (TAligned_Vector<T>::empty()) return S_FALSE;
								
				Release_Item(TAligned_Vector<T>::operator [](index) );
				TAligned_Vector<T>::erase(TAligned_Vector<T>::begin() + index);
				
				return S_OK;
			}

			virtual HRESULT move(const size_t from_index, const size_t to_index) {
				if ((from_index >= TAligned_Vector<T>::size()) ||
					(to_index >= TAligned_Vector<T>::size()) ||
					(from_index == to_index)) return E_INVALIDARG;

				if (from_index < to_index)  
					std::rotate(TAligned_Vector<T>::begin() + from_index, TAligned_Vector<T>::begin() + to_index, TAligned_Vector<T>::begin() + to_index + 1);
				else  //to_index < from_index
					std::rotate(TAligned_Vector<T>::begin() + to_index, TAligned_Vector<T>::begin() + from_index, TAligned_Vector<T>::begin() + from_index+1);
				

				return S_OK;
			}

			virtual HRESULT empty() const override final {
				return TAligned_Vector<T>::empty() ? S_OK : S_FALSE;
			}
		};


		template <typename T>
		class CVector_View : public virtual IVector_Container<T>, public virtual CNotReferenced, public TAligned_Vector<T> {
		protected:
			const T *mBegin, *mEnd;
		public:
			CVector_View(const T *begin, const T *end) : mBegin(begin), mEnd(end) {}

			virtual HRESULT set(T *begin, T *end) override final { return E_NOTIMPL; };
			virtual HRESULT add(T *begin, T *end) override final { return E_NOTIMPL; };
			virtual HRESULT get(T **begin, T **end) const override final { *begin = const_cast<T*>(mBegin); *end = const_cast<T*>(mEnd); return S_OK; }
			virtual HRESULT pop(T* value) override final { return E_NOTIMPL; }
			virtual HRESULT remove(const size_t index) override final { return E_NOTIMPL; }
			virtual HRESULT move(const size_t from_index, const size_t to_index) override final { return E_NOTIMPL; };
			virtual HRESULT empty() const override final { return mEnd<=mBegin ? S_OK : S_FALSE; }
		};

		#pragma warning( pop ) 
	}

	

	template <typename T>
	IVector_Container<T>* Create_Container(T *begin, T *end) {
		IVector_Container<T> *obj = nullptr;
		if (Manufacture_Object<internal::CVector_Container<T>, IVector_Container<T>>(&obj) == S_OK)
			obj->set(begin, end);
		return obj;
	}

	template <typename T>
	class SVector_Container : public std::shared_ptr<IVector_Container<T>> {
	protected:
		T* get_bound(const bool first) const {
			//Do not cache these values as we cannot track every possible modification of the underlying vector.
			//Hence, the code complexity is less or comparable to a stack-constructed solution that would cache both values once needed.
			T* b = nullptr;
			T* e = nullptr;

			if (operator bool()) {
				if (std::shared_ptr<IVector_Container<T>>::get()->get(&b, &e) != S_OK)
					return nullptr;
			}

			return first ? b : e;
		}
	public:
		T* begin() const { return get_bound(true); } ;
		T* end() const { return get_bound(false); };
	};


	template <typename T, typename S = SVector_Container<T>>
	S Create_Container_shared(T *begin, T *end) {
		IVector_Container<T> *obj = Create_Container<T>(begin, end);
		return refcnt::make_shared_reference_ext <S, IVector_Container<T>>(obj, false);
	}

	template <typename T, typename I = IVector_Container<T>>
	I* Copy_Container(I* src) {
		I* result = nullptr;
		// copy parameter hint to internal vector
		T *begin, *end;
		if (src->get(&begin, &end) == S_OK)
			result = Create_Container<T>(begin, end);

		return result;
	}

	template <typename T, typename S = SVector_Container<T>>
	S Copy_Container(SVector_Container<T> src) {
		S result;
		// copy parameter hint to internal vector
		T *begin, *end;
		if (src->get(&begin, &end) == S_OK)
			result = Create_Container_shared<T, S>(begin, end);

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


	class Swstr_container : public virtual std::shared_ptr<refcnt::wstr_container> {
	public:
		Swstr_container& operator=(const Swstr_container&) = default;	//https://stackoverflow.com/questions/34554612/warning-defaulted-move-assignment-operator-of-x-will-move-assign-virtual-base-c
		void set(const wchar_t *str);
		bool operator==(const wchar_t *other) const;
	};
	
}
