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

#include "string_utils.h"

#include <sstream>
#include <algorithm>
#include <cwctype>

std::string Narrow_WString(const std::wstring& wstr) {
	return Narrow_WChar(wstr.c_str());
}

std::string Narrow_WChar(const wchar_t *wstr) {
	std::ostringstream stm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet< std::ctype<wchar_t> >(stm.getloc());

	const size_t len = wcslen(wstr);
	for (size_t i = 0; i < len; ++i) 		
		stm << ctfacet.narrow(wstr[i], 0);
	
	return stm.str();
}

std::wstring Widen_Char(const char *str) {
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t> >(wstm.getloc());
	const size_t len = strlen(str);
	for (size_t i = 0; i < len; ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}


std::wstring WString_To_Lower(const std::wstring& wstr) {
	std::wstring result;

	std::transform(wstr.begin(), wstr.end(), std::back_inserter(result), std::towlower);

	return result;
}