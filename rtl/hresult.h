/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#pragma once

#ifdef _WIN32
  #include <WTypes.h>
#else
  typedef int HRESULT;

  typedef unsigned long	DWORD;
  typedef long			LONG;
  typedef unsigned long	ULONG;
  // define macro to avoid redefinition in several used libraries (e.g. ExcelFormat)
  #define INT32_TYPES_DEFINED

  const HRESULT S_OK = 0;
  const HRESULT S_FALSE = -1;
#ifndef ENOENT
  const HRESULT ENOENT = 2;
#endif
  const HRESULT E_INVALIDARG = 0x80070057;
  const HRESULT E_NOTIMPL = 0x80004001;
  const HRESULT E_UNEXPECTED = 0x8000FFFFL;
  const HRESULT E_FAIL = 0x80004005L;
  const HRESULT E_NOINTERFACE = 0x80004002L;

  #define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
  #define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif
