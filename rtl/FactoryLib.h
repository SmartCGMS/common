/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#pragma once
#include "../iface/DeviceIface.h"
#include "Dynamic_Library.h"

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

		void *resolution = internal::resolve_factory_symbol(symbol_name);
		if (!resolution) resolution = internal::resolve_not_impl_symbol(symbol_name);	//still returning not_impl if cannot load the symbol

		return reinterpret_cast<T>(resolution);
	}
}
