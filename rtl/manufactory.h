#pragma once

#include "hresult.h"


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