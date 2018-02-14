#pragma once

#include "hresult.h"

template <typename T>
HRESULT do_get_descriptors(const std::vector<T> &descriptors, T **begin, T **end) {
	if (!descriptors.empty()) {
		*begin = const_cast<T*>(descriptors.data());
		*end = *begin + descriptors.size();
	}
	else
		begin = end = nullptr;
	return begin != nullptr ? S_OK : S_FALSE;
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
