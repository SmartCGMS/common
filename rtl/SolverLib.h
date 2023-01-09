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

#include "../iface/SolverIface.h"
#include "../rtl/FilterLib.h"

#include <memory>

#if (defined(__cpp_lib_execution) || __has_include(<execution>)) && (!defined(ANDROID) || defined(_LIBCPP_HAS_PARALLEL_ALGORITHMS)) // Android in NDK r25b still does not support parallel STL
#include <execution>
#else
namespace std
{
	// minimal substitute for missing execution header (for pre C++20 compilers)

	enum class execution {
		seq,
		unseq,
		par,
		par_unseq
	};

	template<typename Fnc, typename It>
	void for_each(const execution ex, const It& begin, const It& end, Fnc fnc)
	{
		for (It itr = begin; itr != end; itr++)
			fnc(*itr);
	}

	template<typename Fnc, typename It>
	auto min_element(const execution ex, const It& begin, const It& end, Fnc fnc)
	{
		return std::min_element(begin, end, fnc);
	}
}
#endif

namespace solver {
	template <typename T>
	class CInt_Iterator {
	protected:
		T mVal = 0;
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = T;
		using pointer = std::add_pointer_t<T>;
		using reference = T&;
	public:
		CInt_Iterator(const T val) : mVal(val) {};

		CInt_Iterator& operator++() { ++mVal; return *this; }
		CInt_Iterator operator++(int) { CInt_Iterator tmp(*this); ++mVal; return tmp; }

		bool operator==(CInt_Iterator const& other) const { return mVal == other.mVal; }
		bool operator!=(CInt_Iterator const& other) const { return mVal != other.mVal; }

		T operator*() const { return mVal; }
		pointer operator->() const { return &mVal; }
	public:
		CInt_Iterator() = default;
		CInt_Iterator(CInt_Iterator&&) = default;
		CInt_Iterator(CInt_Iterator const&) = default;
		CInt_Iterator& operator=(CInt_Iterator&&) = default;
		CInt_Iterator& operator=(CInt_Iterator const&) = default;

	};

	solver::TSolver_Setup Check_Default_Parameters(const solver::TSolver_Setup &setup, const size_t default_max_generations, const size_t default_population_size);	
	HRESULT Solve_Generic(const GUID& solver_id, const solver::TSolver_Setup& setup, solver::TSolver_Progress& progress) noexcept;

	template <typename T>
	inline double Solution_Distance(const size_t objective_count, const T solution) {
		if (objective_count == 1)
			return solution[0];

		double result = 0.0;
		for (size_t i = 0; i < objective_count; i++) {
			result += solution[i] * solution[i];
		}

		return result;
	}

}

namespace scgms {
	class SMetric : public std::shared_ptr<IMetric> {
	private:
		void Init(const scgms::TMetric_Parameters &params);
	public:
		SMetric();
		SMetric(const TMetric_Parameters &params);
		SMetric Clone();
	};

	class SCalculate_Filter_Inspection : public std::shared_ptr<ICalculate_Filter_Inspection> {
	public:
		SCalculate_Filter_Inspection() noexcept {};
		SCalculate_Filter_Inspection(const SFilter &calculate_filter);
	};


	HRESULT Optimize_Parameters(scgms::SFilter_Chain_Configuration configuration, const size_t *filter_indices, const wchar_t **parameters_configuration_names, size_t filter_count,
								scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data,
								const GUID &solver_id, const size_t population_size, const size_t max_generations, 
								const double** hints, const size_t hint_count,
								solver::TSolver_Progress &progress,
								refcnt::Swstr_list error_description);

}
