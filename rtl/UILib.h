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

#include "../iface/UIIface.h"

#include <map>

namespace glucose
{
	std::vector<TModel_Descriptor> get_model_descriptors();
	std::vector<TMetric_Descriptor> get_metric_descriptors();
	std::vector<TSolver_Descriptor> get_solver_descriptors();

	bool get_model_descriptor_by_id(const GUID &id, TModel_Descriptor &desc);
	bool get_model_descriptor_by_signal_id(const GUID &signal_id, TModel_Descriptor &desc);

	extern const std::array<const wchar_t*, static_cast<size_t>(glucose::NDevice_Event_Code::count)> event_code_text;
	
	class CSignal_Names {
			//should we replace this conversion class with a simple function, the map would have to use TBB allocator to avoid memory leaks
			//and because we don't want TBB to be a required component to compile all filters, we rather ask the programmer to instantitate
			//this class and disposes once unneeded to prevent memory leaks
	protected:
		std::map<GUID, std::wstring> mSignal_Names;
	public:
		CSignal_Names();
		std::wstring Get_Name(const GUID &signal_id);
	};
	
}

GUID WString_To_GUID(const std::wstring& str);
std::wstring GUID_To_WString(const GUID &guid);
std::wstring Get_Padded_Number(uint32_t num, size_t places);
