#pragma once

#include "hresult.h"

template <typename T>
HRESULT do_get_descriptors(const std::vector<T> &descriptors, T **begin, T **end) {
	*begin = const_cast<T*>(descriptors.data());
	*end = *begin + descriptors.size();
	return S_OK;
}