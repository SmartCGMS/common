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
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#if defined(_WIN32) && !defined(_WIN64)
	#define IfaceCalling __stdcall
#else
	#define IfaceCalling
#endif

#include <memory>

#include "../rtl/hresult.h"
#include "../rtl/guid.h"

/* Note that we have to split this class into interface and its implementation,
 * as we can use several different compilers. This is to guarantee that the method
 * implementations will be consistent - i.e. that each object will have the methods
 * compiled with the same compiler.
 */
namespace refcnt {

	/* an object that is held by a single owner (equivalent to std::unique_ptr) */
	class IUnique_Reference {
		public:
			/* releases allocated memory/object returns 0 */
			virtual ULONG IfaceCalling Release() = 0;
	};

	/* a reference-counted object with possibly multiple owners (equivalent to std::shared_ptr)
	 *
	 * Actually, this is IUnknown of the WinAPI's COM
	 * HRESULT and ULONG are used to allow possible and easy interoperability
	 * accross different compilers and languages on Windows */
	class IReferenced {
		public:
			/* queries the object for support of the given interface identified by GUID parameter riid
			 * if the interface is supported, the object casts itself to that interface and fills the ppvObj pointer
			 * in that case, S_OK is retured; otherwise, E_NOTIMPL is returned */
			virtual HRESULT IfaceCalling QueryInterface(const GUID* riid, void **ppvObj) = 0;

			/* adds a reference to the reference counter; returns a number of references after this call */
			virtual ULONG IfaceCalling AddRef() = 0;
			/* releases a reference, releases the allocated memory/object if the reference count drops to zero; returns a number of references after this call */
			virtual ULONG IfaceCalling Release() = 0;
	};

	/* factory function to create std::shared_ptr holding an object implementing IReferenced
	 * S is a std::shared_ptr or its child class, I is the IReferenced type to be created
	 * note that shared_ptr will overtake the assignment operations and maintain its own counter
	 * when shared_ptr's counter reaches zero, the Release method is called automatically thanks to a custom deleter */
	template <typename S, typename I>
	S make_shared_reference_ext(I *obj, bool add_reference) {
		if ((add_reference) && (obj != nullptr)) {
			obj->AddRef();
		}

		S result;
		result.reset(obj, [](I* obj_to_release) {
			if (obj_to_release != nullptr) {
				obj_to_release->Release();
			}
		});

		return result;
	}

	/* are all objects given as parameters non-nullptr? */
	template <typename T>
	bool Shared_Valid_All(const T& a) {
		return a.get() != nullptr;
	}

	/* are all objects given as parameters non-nullptr? */
	template <typename T, typename... Args>
	bool Shared_Valid_All(const T& a, const Args&... args) {
		bool result = a.get() != nullptr;

		if (result) {
			result &= Shared_Valid_All(args...);
		}
		return result;
	}

	/* interop wrapper interface for continuous dynamic memory array */
	template <typename T>
	class IVector_Container : public virtual refcnt::IReferenced {
		public:
			/* sets new content to a given block for <begin, end); T may point to IReferenced, thus cannot be const */
			virtual HRESULT IfaceCalling set(T *begin, T *end) = 0;
			/* appends new content to the container */
			virtual HRESULT IfaceCalling add(T *begin, T *end) = 0;
			/* retrieves memory boundaries marked with <*begin, *end) */
			virtual HRESULT IfaceCalling get(T **begin, T **end) const = 0;
			/* removes and returns exactly one item from the container items, if there is any */
			virtual HRESULT IfaceCalling pop(T* value) = 0;
			/* removes exacly one item from the existing content, identified by its ordinal number/position from the beginning */
			virtual HRESULT IfaceCalling remove(const size_t index) = 0;
			/* moves exactly one item from source to destination index */
			virtual HRESULT IfaceCalling move(const size_t from_index, const size_t to_index) = 0;
			/* returns S_OK if the container is empty, S_FALSE if not */
			virtual HRESULT IfaceCalling empty() const = 0;
	};

	using str_container = IVector_Container<char>;
	using wstr_container = IVector_Container<wchar_t>;
	using wstr_list = IVector_Container<wstr_container*>;
	using double_container = IVector_Container<double>;	
	using byte_container = IVector_Container<uint8_t>;
}
