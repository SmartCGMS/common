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
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#pragma once

#include "../iface/FilterIface.h"

#include <cmath>
#include <vector>
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
Is_Any_NaN(const T value) {
    return std::isnan(value);
}


template <typename T>
typename std::enable_if<!std::is_floating_point<T>::value, bool>::type
Is_Any_NaN(const T& list) {
    for (const auto& elem : list)
        if (Is_Any_NaN(elem)) return true;

    return false;
}

template <typename T, typename... Args>
bool Is_Any_NaN(const T value, const Args&... args) {
    return Is_Any_NaN(value) || Is_Any_NaN(args...);
}


//does not check whether series contains NaN, destroys series content
bool Calculate_Signal_Stats(std::vector<double>& series, scgms::TSignal_Stats& signal_error);