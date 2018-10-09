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

#include "Common_Calculation.h"

CCommon_Calculation::CCommon_Calculation(glucose::WTime_Segment segment, const GUID &source_signal) : mSource_Signal(segment.Get_Signal(source_signal)) {
	if (!refcnt::Shared_Valid_All(mSource_Signal)) throw std::exception{};
}

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const {
	return E_NOTIMPL;
}

HRESULT IfaceCalling CCommon_Calculation::Get_Discrete_Bounds(glucose::TBounds* const time_bounds, glucose::TBounds* const level_bounds, size_t *level_count) const {
	return E_NOTIMPL;
}

HRESULT IfaceCalling CCommon_Calculation::Add_Levels(const double *times, const double *levels, const size_t count) {
	return E_NOTIMPL;	//we calculate from a signal that can possibly add new levels
}