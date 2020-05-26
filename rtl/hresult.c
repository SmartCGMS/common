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

#include "hresult.h"



#ifdef _WIN32
	#include <WTypes.h>
#else
	typedef long HRESULT;

	typedef unsigned long	DWORD;
	typedef long			LONG;
	typedef unsigned long	ULONG;
	// define macro to avoid redefinition in several used libraries (e.g. ExcelFormat)
	#define INT32_TYPES_DEFINED

	#define S_OK									((HRESULT)0L)
	#define S_FALSE									((HRESULT)1L)
	#define E_INVALIDARG							((HRESULT)0x80070057)
	#define E_NOTIMPL								((HRESULT)0x80004001)
	#define E_UNEXPECTED							((HRESULT)0x8000FFFFL)
	#define E_FAIL									((HRESULT)0x80004005L)
	#define E_NOINTERFACE							((HRESULT)0x80004002L)
	#define E_ABORT									((HRESULT)0x80004004L)
	#define E_ILLEGAL_METHOD_CALL					((HRESULT)0x8000000EL)
	#define E_ILLEGAL_STATE_CHANGE					((HRESULT)0x8000000DL)
	#define ERROR_DS_DRA_EXTN_CONNECTION_FAILED		((HRESULT)8466L)
	#define E_ACCESSDENIED							((HRESULT)0x80070005L)
	#define E_NOT_SET								((HRESULT)0x80070490L)
	#define MK_E_CANTOPENFILE						((HRESULT)0x800401EAL)
	#define CO_E_ERRORINDLL							((HRESULT)0x800401F9L)

	#ifndef ENOENT
		#define ENOENT								((HRESULT)2L)
	#endif

	#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
	#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif



wchar_t* Describe_Error(const HRESULT error) {
	//although we could use the system error description, at least on Win32,
	//we provide the meanings as we use them

	
	switch (error) {
		case S_OK: return L"No error"; break;
		case S_FALSE: return L"Succeeded, but no result"; break;
		case E_INVALIDARG: return L"One or more invalid argument(s)"; break;
		case E_NOTIMPL: return L"Not implemented"; break;
		case E_UNEXPECTED: return L"Catastrophic failure"; break;
		case E_FAIL: return L"Unspecified error"; break;
		case E_NOINTERFACE: return L"No such interface"; break;
		case E_ABORT: return L"Operation aborted"; break;
		case E_ILLEGAL_METHOD_CALL: return L"Method cannot be called at this time"; break;
		case E_ILLEGAL_STATE_CHANGE: return L"Attempted illegal state change"; break;
		case ERROR_DS_DRA_EXTN_CONNECTION_FAILED: return L"No object to complete the operation"; break;
		case E_ACCESSDENIED: return L"Access denied"; break;
		case /*E_NOT_SET*/((HRESULT)0x80070490L): return L"Not set/found"; break;	//otherwise fails to compile on Win
		case MK_E_CANTOPENFILE: return L"Cannot open file"; break;
		case CO_E_ERRORINDLL: return L"Dynamic-libray error"; break;
		case ENOENT: return L"No entry"; break;
		default:	return L"Unknown error"; break;
	}
}

