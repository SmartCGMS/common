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


#include "DeviceLib.h"
#include "referencedImpl.h"
#include "Buffer_Pool.h"
#include "Eigen_Buffer_Pool.h"


#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance


class CCommon_Calculation : public virtual glucose::ISignal, public virtual refcnt::CReferenced {
protected:
	template <typename T>
	T& Convert_Parameters(glucose::IModel_Parameter_Vector *params, const double *default_parameters) const {
		double *begin{ const_cast<double*>(default_parameters) };	//just in case that no parameters are set at all -> than we have to use the default ones
		if (params) {
			double *tmp_begin, *end;
			if (params->get(&tmp_begin, &end) == S_OK) {		
				//not that params still could be empty
				if (tmp_begin && (tmp_begin != end))  begin = tmp_begin;
			}
		}
	
		T &result = *(reinterpret_cast<T*>(begin));
		return result;
	}
protected:
	mutable CBuffer_Pool<TVector1D> mVector1D_Pool{Eigen_Buffer_Pool_Resize<TVector1D> };
protected:
	glucose::SSignal mSource_Signal; //TODO: remove as CTimeSegment treats any incoming level as measured signal
public:	
	CCommon_Calculation() {};
	CCommon_Calculation(glucose::WTime_Segment segment, const GUID &source_signal);
	virtual ~CCommon_Calculation() {};

	//glucose::ISignal iface
	virtual HRESULT IfaceCalling Get_Discrete_Levels(double* const times, double* const levels, const size_t count, size_t *filled) const override;
	virtual HRESULT IfaceCalling Get_Discrete_Bounds(glucose::TBounds* const time_bounds, glucose::TBounds* const level_bounds, size_t *level_count) const override;
	virtual HRESULT IfaceCalling Add_Levels(const double *times, const double *levels, const size_t count) override;
	
};

#pragma warning( pop )