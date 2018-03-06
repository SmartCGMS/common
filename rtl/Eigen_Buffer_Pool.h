#pragma once

#include <Eigen/Dense>

using TVector1D = Eigen::Array<double, 1, Eigen::Dynamic, Eigen::RowMajor>;

template <typename T>
void Eigen_Buffer_Pool_Resize(T &vector, const size_t minimum_size) {
	if (vector.cols() < static_cast<int>(minimum_size)) vector.resize(Eigen::NoChange, static_cast<int>(minimum_size));
}

template <typename T>
Eigen::Map<T> Map_Double_To_Eigen(const double* vector, const size_t count) {
	return Eigen::Map<T> { const_cast<double*>(vector), T::RowsAtCompileTime, static_cast<Eigen::Index>(count) };
}

template <typename T>
Eigen::Map<T> Map_Double_To_Eigen(double* const vector, const size_t count) {
	return Eigen::Map<T> { vector, T::RowsAtCompileTime, static_cast<Eigen::Index>(count) };
}