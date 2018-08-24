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
