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

#include "../rtl/guid.h"
#include "referencedIface.h"
#include "DeviceIface.h"

namespace glucose
{
	using IApprox_Parameters_Vector = refcnt::double_container;

	class IApproximator : public virtual refcnt::IReferenced
	{
		public:
			// parameters of approximation curve(s) on given signal (signal is passed in constructor of each approximation) are calculated when
			// performing construction of approximator object;

			// approximates signal levels (or derivations) on given times array of size "count"
			// a call to Approximate method must precede (once per ISignal data set)
			virtual HRESULT IfaceCalling GetLevels(const double* times, double* const levels, const size_t count, const size_t derivation_order) = 0;
	};

	using TCreate_Approximator = HRESULT(IfaceCalling *)(const GUID *approx_id, ISignal *signal, IApprox_Parameters_Vector* configuration, IApproximator **approx);
}
