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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

#include "hresult.h"
#include "../iface/EmbeddedIface.h"
#include "../lang/dstrings.h"

template <typename T, typename... Args>
T* Create_Raw_Object(Args... args) {
	T* result = new T();
	if (result) {
		const auto err = result->Initialize(args...);
		if (!Succeeded(err.code)) {
			delete result;
			result = nullptr;
		}
	}

	return result;		
}

template <typename T, typename... Args>
std::tuple <HRESULT, std::unique_ptr<T>, wchar_t*> Create_Unique_Object(Args... args) {
	HRESULT rc E_UNEXPECTED;
	wchar_t* desc = nullptr;
	
	std::unique_ptr<T> obj = std::make_unique<T>();
	if (obj) {
		const auto err = obj->Initialize(args...);

		rc = err.code;
		desc = err.description;
	}
	else {
		rc = E_OUTOFMEMORY;
		desc = const_cast<wchar_t*>(rsFailed_to_allocate_memory);
	}

	
	return { rc, std::move(obj), desc };;
}

template <class T, class I, typename... Args>
TEmbedded_Error Manufacture_Object(I** manufactured, Args... args) {
	//As SCGMS is supposed to run on low-power devices and real-time systems, there's no exception handling
	//and we provide the new operator has tor return nullptr instead of throwing.
	//hence, we enforce the use of initialization method

	auto [rc, obj, desc] = Create_Unique_Object<T>(args...);

	if (Succeeded(rc)) {
		(*manufactured) = static_cast<I*>(obj.get());
		(*manufactured)->AddRef();
		obj.release();
	}

	return { rc, desc };
}

