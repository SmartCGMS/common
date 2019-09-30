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
 * Univerzitni 8
 * 301 00, Pilsen
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

#include "DeviceLib.h"
#include "../iface/UIIface.h"

#include "referencedImpl.h"
#include <vector>
#include <string>
#include <functional>

namespace glucose {

	using SFilter = refcnt::SReferenced<IFilter>;

	class SFilter_Parameter : public virtual refcnt::SReferenced<glucose::IFilter_Parameter> {
	public:
		std::wstring as_string(HRESULT &rc);
		void set_string(const wchar_t *str, HRESULT &rc);

		int64_t as_int(HRESULT &rc);
		void set_int(const int64_t value, HRESULT &rc);

		double as_double(HRESULT &rc);
		void set_double(const double value, HRESULT &rc);

		bool as_bool(HRESULT &rc);
		void set_bool(const bool value, HRESULT &rc);
	};

	class SFilter_Configuration_Link : public virtual refcnt::SReferenced<IFilter_Configuration_Link> {
	public:
		TFilter_Descriptor descriptor();
	};

	class SPersistent_Filter_Chain_Configuration : public virtual refcnt::SReferenced<glucose::IPersistent_Filter_Chain_Configuration> {
	public:
		SPersistent_Filter_Chain_Configuration(); 
		
		void for_each(std::function<void(glucose::SFilter_Configuration_Link)> callback);
	};

	class SFilter_Executor : public virtual refcnt::SReferenced<glucose::IFilter_Executor> {
	public:
		SFilter_Executor() : refcnt::SReferenced<glucose::IFilter_Executor>() {};
		SFilter_Executor(SPersistent_Filter_Chain_Configuration configuration, glucose::TOn_Filter_Created on_filter_created, const void* on_filter_created_data);

		HRESULT Execute(glucose::UDevice_Event event);
	};


	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);


	class SFilter_Configuration : public std::shared_ptr<glucose::IFilter_Configuration> {
	protected:
		template <typename T, typename F>	//F== std::function < HRESULT(refcnt::SReferenced<glucose::IFilter_Parameter>, T &) > ?
		T Read_Parameter(const wchar_t *name, F func, T default_value) const {
			refcnt::SReferenced<glucose::IFilter_Parameter> parameter = Resolve_Parameter(name);
			if (!parameter) return default_value;

			T value;
			if (func(parameter, value) != S_OK) return default_value;

			return value;
		}
	public:
		std::wstring Read_String(const wchar_t* name, const std::wstring& default_value = {}) const;
		int64_t Read_Int(const wchar_t* name, const int64_t default_value = std::numeric_limits<int64_t>::max()) const;
		std::vector<int64_t> Read_Int_Array(const wchar_t* name) const;
		GUID Read_GUID(const wchar_t* name, const GUID &default_value = Invalid_GUID) const;
		bool Read_Bool(const wchar_t* name, bool default_value = false) const;
		double Read_Double(const wchar_t* name, const double default_value = std::numeric_limits<double>::quiet_NaN()) const;
		void Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) const;

		refcnt::SReferenced<glucose::IFilter_Parameter> Resolve_Parameter(const wchar_t* name) const;
	};



	#pragma warning( push )
	#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

	class CBase_Filter : public virtual glucose::IFilter, public virtual refcnt::CReferenced {
	protected:
		glucose::SFilter mOutput;	//aka the next_filter
		HRESULT Send(glucose::UDevice_Event &event);
	protected:
		//Descending class is supposed to implement these two methods only
		virtual HRESULT Do_Execute(glucose::UDevice_Event event) = 0;
		virtual HRESULT Do_Configure(glucose::SFilter_Configuration configuration) = 0;
	public:
		CBase_Filter(glucose::IFilter *output);
		virtual ~CBase_Filter();
		virtual HRESULT IfaceCalling Configure(IFilter_Configuration* configuration) override final;
		virtual HRESULT IfaceCalling Execute(glucose::IDevice_Event *event) override final;
	};

	#pragma warning( pop )


	class SError_Filter_Inspection : public std::shared_ptr<IError_Filter_Inspection> {
	public:
		SError_Filter_Inspection() noexcept {};
		SError_Filter_Inspection(SFilter &error_filter);
	};

	class SDrawing_Filter_Inspection : public std::shared_ptr<IDrawing_Filter_Inspection> {
	public:
		SDrawing_Filter_Inspection() noexcept {};
		SDrawing_Filter_Inspection(SFilter &drawing_filter);
	};

	class SLog_Filter_Inspection : public std::shared_ptr<ILog_Filter_Inspection> {
	public:
		SLog_Filter_Inspection() noexcept {};
		SLog_Filter_Inspection(SFilter &log_filter);
		bool pop(std::shared_ptr<refcnt::wstr_list> &list);
	};

	class SCalculate_Filter_Inspection : public std::shared_ptr<ICalculate_Filter_Inspection> {
	public:
		SCalculate_Filter_Inspection() noexcept {};
		SCalculate_Filter_Inspection(SFilter &calculate_filter);
	};

}


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container);
glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
