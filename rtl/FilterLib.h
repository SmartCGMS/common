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
		NParameter_Type type();

		std::wstring as_wstring(HRESULT &rc);
		HRESULT set_wstring(const wchar_t *str);		

		int64_t as_int(HRESULT &rc);		

		double as_double(HRESULT &rc);		
		std::vector<double> as_double_array(HRESULT &rc);
		HRESULT set_double_array(const std::vector<double> &values);
		HRESULT double_array_from_wstring(const wchar_t *str_value);

		bool as_bool(HRESULT &rc);
		HRESULT set_bool(const bool value);

		GUID as_guid(HRESULT &rc);		
		HRESULT guid_from_wstring(const wchar_t *str_value);

		std::vector<int64_t> as_int_array(HRESULT &rc);
		HRESULT set_int_array(const std::vector<int64_t> &values);
		HRESULT int_array_from_wstring(const wchar_t *str_value);	
	};

	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);


	//class SFilter_Configuration : public std::shared_ptr<glucose::IFilter_Configuration> {
	namespace internal {

		template <typename IConfiguration>
		class CInternal_Filter_Configuration : public virtual refcnt::SReferenced<IConfiguration> {
		protected:
			template <typename T, typename M>
			T Read_Parameter(const wchar_t *name, M method, T default_value) const {
			//T Read_Parameter(const wchar_t *name, T (glucose::SFilter_Parameter::*method)(HRESULT), T default_value) const {
				SFilter_Parameter parameter = Resolve_Parameter(name);
				if (!parameter) return default_value;

				HRESULT rc;
				T value = ((&parameter)->*method)(rc);
				if (rc != S_OK) return default_value;

				return value;
			}
		public:
			virtual ~CInternal_Filter_Configuration() {};

			std::wstring Read_String(const wchar_t* name, const std::wstring& default_value = {}) const {
				return Read_Parameter<std::wstring>(name, &SFilter_Parameter::as_wstring, default_value);
			}

			int64_t Read_Int(const wchar_t* name, const int64_t default_value = std::numeric_limits<int64_t>::max()) const {
				return Read_Parameter<int64_t>(name, &SFilter_Parameter::as_int, default_value);
			}

			std::vector<int64_t> Read_Int_Array(const wchar_t* name) const {
				return Read_Parameter<std::vector<int64_t>>(name, &SFilter_Parameter::as_int_array, std::vector<int64_t>{});
			}


			GUID Read_GUID(const wchar_t* name, const GUID &default_value = Invalid_GUID) const {
				return Read_Parameter<GUID>(name, &SFilter_Parameter::as_guid, default_value);
			}
			
			bool Read_Bool(const wchar_t* name, bool default_value = false) const {				
				return Read_Parameter<bool>(name, &SFilter_Parameter::as_bool, default_value);				
			}

			double Read_Double(const wchar_t* name, const double default_value = std::numeric_limits<double>::quiet_NaN()) const {
				return Read_Parameter<double>(name, &SFilter_Parameter::as_double, default_value);
			}

			void Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) const {
				const auto parameter = Resolve_Parameter(name);

				bool success = parameter.operator bool();

				if (success) {
					glucose::IModel_Parameter_Vector *raw_parameters;
					if (parameter->Get_Model_Parameters(&raw_parameters) == S_OK) {

						double *begin, *end;
						if (raw_parameters->get(&begin, &end) == S_OK) {
							if ((begin != nullptr) && (begin != end)) {
								const size_t distance = std::distance(begin, end);
								if (distance % 3 == 0) {
									const size_t paramcnt = distance / 3; // lower, default, upper
									lower_bound = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(begin, &begin[paramcnt]);
									default_parameters = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(&begin[paramcnt], &begin[2 * paramcnt]);
									upper_bound = refcnt::Create_Container_shared<double, glucose::SModel_Parameter_Vector>(&begin[2 * paramcnt], &begin[3 * paramcnt]);
									success = true;
								}
							}
						}
					}
					raw_parameters->Release();
				}

				if (!success) {
					lower_bound = glucose::SModel_Parameter_Vector{};
					default_parameters = glucose::SModel_Parameter_Vector{};
					upper_bound = glucose::SModel_Parameter_Vector{};
				}

			}

			SFilter_Parameter Resolve_Parameter(const wchar_t* name) const {
				SFilter_Parameter result;

				if (refcnt::SReferenced<IConfiguration>::operator bool()) {
					glucose::IFilter_Parameter **cbegin, **cend;					
					if (refcnt::SReferenced<IConfiguration>::get()->get(&cbegin, &cend) == S_OK)

						for (glucose::IFilter_Parameter** cur = cbegin; cur < cend; cur++) {
							wchar_t* conf_name;
							if ((*cur)->Get_Config_Name(&conf_name) == S_OK) {
								if (wcscmp(conf_name, name) == 0) {
									result = refcnt::make_shared_reference_ext<SFilter_Parameter, glucose::IFilter_Parameter>(*cur, true);
								}
							}
						}
				}

				return result;	//not found
			}

			void for_each(std::function<void(glucose::SFilter_Parameter)> callback) {
				glucose::IFilter_Parameter **begin, **end;
				HRESULT rc = refcnt::SReferenced<IConfiguration>::get()->get(&begin, &end);
				if (rc != S_OK) return;


				for (; *begin != *end; begin++)
					callback(refcnt::make_shared_reference_ext<SFilter_Parameter, IFilter_Parameter>(*begin, true));
			}
		};
	}

	using SFilter_Configuration = internal::CInternal_Filter_Configuration<IFilter_Configuration>;

	class SFilter_Configuration_Link : public virtual internal::CInternal_Filter_Configuration<IFilter_Configuration_Link> {
	public:
		TFilter_Descriptor descriptor();
		SFilter_Parameter Add_Parameter(const glucose::NParameter_Type type, const wchar_t *conf_name);
	};

	class SPersistent_Filter_Chain_Configuration : public virtual refcnt::SReferenced<glucose::IPersistent_Filter_Chain_Configuration> {
	public:
		SPersistent_Filter_Chain_Configuration();

		void for_each(std::function<void(glucose::SFilter_Configuration_Link)> callback);
	};

	class SFilter_Executor : public virtual refcnt::SReferenced<glucose::IFilter_Executor> {
	public:
		SFilter_Executor() : refcnt::SReferenced<glucose::IFilter_Executor>() {};
		SFilter_Executor(refcnt::SReferenced<glucose::IFilter_Chain_Configuration> configuration, glucose::TOn_Filter_Created on_filter_created, const void* on_filter_created_data);

		HRESULT Execute(glucose::UDevice_Event event);
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
