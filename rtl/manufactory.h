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
#include "referencedImpl.h"


template <class T, class I, typename... Args>
HRESULT Manufacture_Object(I** manufactured, Args... args) {
	HRESULT rc = E_UNEXPECTED;

	try {
		T *tmp = new T(args...);
		(*manufactured) = static_cast<I*> (tmp);
		(*manufactured)->AddRef();

		rc = S_OK;
	}
	catch (...) {
		rc = E_FAIL;
	}


	return rc;
}


template <class T, class I, class S, typename... Args>
S Manufacture_Object_Shared(Args... args) {
	I* manufactured;
	S result;

	if (Manufacture_Object<T, I>(&manufactured, args...) == S_OK)
		result = refcnt::make_shared_reference_ext<S, I>(manufactured, false);
	
	return result;
}
