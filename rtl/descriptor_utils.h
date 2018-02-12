#pragma once

#include "hresult.h"

template <typename T>
HRESULT do_get_descriptors(const std::vector<T> &descriptors, T **begin, T **end) {
	*begin = const_cast<T*>(descriptors.data());
	*end = *begin + descriptors.size();
	return S_OK;
}

constexpr glucose::TSolver_Descriptor Describe_Non_Specialized_Solver(const GUID id, const wchar_t* desc) {
	return glucose::TSolver_Descriptor{
		id,
		desc,
		false,
		0,
		nullptr
	};
}
