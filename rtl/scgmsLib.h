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

#include "../iface/DeviceIface.h"
#include "../iface/FilterIface.h"
#include "Dynamic_Library.h"

namespace scgms {

    namespace factory {
        namespace internal {

            constexpr bool import_test_fails = false;

            void* resolve_factory_symbol(const char* symbol_name);
            void* resolve_not_impl_symbol(const char* symbol_name);
        }

        template <typename T>
        T resolve_symbol(const char* symbol_name) {
            if (internal::import_test_fails)
                return reinterpret_cast<T>(internal::resolve_not_impl_symbol(symbol_name));

            void* resolution = internal::resolve_factory_symbol(symbol_name);
            if (!resolution) resolution = internal::resolve_not_impl_symbol(symbol_name);	//still returning not_impl if cannot load the symbol

            return reinterpret_cast<T>(resolution);
        }
    }


	bool is_scgms_loaded();
}