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

#include <Eigen/Dense>
#include <utility>

using TVector1D = Eigen::Array<double, 1, Eigen::Dynamic, Eigen::RowMajor>;
using TBlock1D = decltype(std::declval<TVector1D>().head(0));

inline bool Is_Any_NaN(const TBlock1D &value) {
	return (value == value).all();	
}


template <typename T>
TBlock1D Reserve_Eigen_Buffer(T &vector, const size_t effective_size) {
	if (vector.cols() < static_cast<int>(effective_size)) vector.resize(Eigen::NoChange, static_cast<int>(effective_size));

	return vector.head(static_cast<int>(effective_size));
}

template <typename T>
Eigen::Map<T> Map_Double_To_Eigen(const double* vector, const size_t count) {
	return Eigen::Map<T> { const_cast<double*>(vector), T::RowsAtCompileTime, static_cast<Eigen::Index>(count) };
}

template <typename T>
Eigen::Map<T> Map_Double_To_Eigen(double* const vector, const size_t count) {
	return Eigen::Map<T> { vector, T::RowsAtCompileTime, static_cast<Eigen::Index>(count) };
}

template <typename T>
Eigen::Map<T> Map_Double_To_Eigen(const double* vector) {
	return Eigen::Map<T> { const_cast<double*>(vector), T::RowsAtCompileTime, T::ColsAtCompileTime };
}
