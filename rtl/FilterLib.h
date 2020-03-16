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

#include "DeviceLib.h"
#include "../iface/UIIface.h"
#include "../lang/dstrings.h"

#include <vector>
#include <string>
#include <functional>

namespace scgms {

	using SFilter = refcnt::SReferenced<IFilter>;

	class SFilter_Parameter : public virtual refcnt::SReferenced<scgms::IFilter_Parameter> {
	public:
		NParameter_Type type();
		const wchar_t* configuration_name();

		std::wstring as_wstring(HRESULT &rc, bool read_interpreted);
		HRESULT set_wstring(const wchar_t *str);		

		int64_t as_int(HRESULT &rc);		

		double as_double(HRESULT &rc);		
		std::vector<double> as_double_array(HRESULT &rc);
		HRESULT set_double_array(const std::vector<double> &values);
		HRESULT double_array_from_wstring(const wchar_t *str_value);

		bool as_bool(HRESULT &rc);
		HRESULT set_bool(const bool value);

		GUID as_guid(HRESULT &rc);		
		HRESULT set_GUID(const GUID &guid);

		std::vector<int64_t> as_int_array(HRESULT &rc);
		HRESULT set_int_array(const std::vector<int64_t> &values);
		HRESULT int_array_from_wstring(const wchar_t *str_value);	
	};

	bool add_filters(const std::vector<scgms::TFilter_Descriptor> &descriptors, scgms::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);


	//class SFilter_Configuration : public std::shared_ptr<scgms::IFilter_Configuration> {
	namespace internal {

		SFilter_Parameter Create_Filter_Parameter(const scgms::NParameter_Type type, const wchar_t *config_name);

		template <typename IConfiguration>
		class CInternal_Filter_Configuration : public virtual refcnt::SReferenced<IConfiguration> {
		protected:
			template <typename T, typename M, typename... TArgs>
			T Read_Parameter(const wchar_t *name, M method, T default_value, TArgs... args) const {				
				SFilter_Parameter parameter = Resolve_Parameter(name);
				if (!parameter) return default_value;
				
				HRESULT rc = E_FAIL;
				T value = ((&parameter)->*method)(rc, args...);
				if (rc != S_OK) return default_value;

				return value;
			}
		public:
			virtual ~CInternal_Filter_Configuration() {};

			std::wstring Read_String(const wchar_t* name, bool read_interpreted = true, const std::wstring& default_value = {}) const {
				return Read_Parameter<std::wstring>(name, &SFilter_Parameter::as_wstring, default_value, read_interpreted);
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

			std::vector<double> Read_Double_Array(const wchar_t* name) const {
				return Read_Parameter<std::vector<double>>(name, &SFilter_Parameter::as_double_array, std::vector<double>{});
			}


			bool Read_Parameters(const wchar_t* name, std::vector<double> &lower_bound, std::vector<double> &default_parameters, std::vector<double> &upper_bound) const {

				bool success = false;

				const auto parameter = Resolve_Parameter(name);
				if (parameter) {					
					scgms::IModel_Parameter_Vector *raw_parameters;
					if (parameter->Get_Model_Parameters(&raw_parameters) == S_OK) {
						
						double *begin, *end;
						if (raw_parameters->get(&begin, &end) == S_OK) {
							if ((begin != nullptr) && (begin != end)) {
								const size_t distance = std::distance(begin, end);
								if (distance % 3 == 0) {
									const size_t paramcnt = distance / 3; // lower, default, upper
									lower_bound.assign(begin, begin+paramcnt);
									default_parameters.assign(begin+paramcnt, begin + 2*paramcnt);
									upper_bound.assign(begin + 2*paramcnt, begin + 3*paramcnt);
									success = true;
								}
							}
						}
					}
					raw_parameters->Release();
				}

				return success;
			}

			bool Write_Parameters(const wchar_t* name, const std::vector<double> &lower_bound, const std::vector<double> &default_parameters, const std::vector<double>& upper_bound) {
				bool success = (lower_bound.size() == default_parameters.size()) && (default_parameters.size() == upper_bound.size());
				
				if (success) {
					auto parameter = Resolve_Parameter(name);
					if (!parameter) {
						parameter = Create_Filter_Parameter(scgms::NParameter_Type::ptDouble_Array, name);
						success = parameter.operator bool();

						if (success) {
							scgms::IFilter_Parameter *raw_parameter = parameter.get();
							success = refcnt::SReferenced<IConfiguration>::get()->add(&raw_parameter, &raw_parameter + 1) == S_OK;
						}
					}

					

					if (success) {
						std::vector<double> aggregated;
						aggregated.assign(lower_bound.begin(), lower_bound.end());
						aggregated.insert(aggregated.end(), default_parameters.begin(), default_parameters.end());
						aggregated.insert(aggregated.end(), upper_bound.begin(), upper_bound.end());

						success = parameter.set_double_array(aggregated) == S_OK;
					}
				}

				return success;				
			}

			SFilter_Parameter Resolve_Parameter(const wchar_t* name) const {
				SFilter_Parameter result;				

				if (name) {
					if (refcnt::SReferenced<IConfiguration>::operator bool()) {
						scgms::IFilter_Parameter **cbegin, **cend;
						if (refcnt::SReferenced<IConfiguration>::get()->get(&cbegin, &cend) == S_OK)

							for (scgms::IFilter_Parameter** cur = cbegin; cur != cend; cur++) {
								wchar_t* conf_name;
								if ((*cur)->Get_Config_Name(&conf_name) == S_OK) {
									if (wcscmp(conf_name, name) == 0) {
										result = refcnt::make_shared_reference_ext<SFilter_Parameter, scgms::IFilter_Parameter>(*cur, true);
									}
								}
							}
					}
				}

				return result;	//not found
			}

			void for_each(std::function<void(scgms::SFilter_Parameter)> callback) {
				scgms::IFilter_Parameter **begin, **end;
				HRESULT rc = refcnt::SReferenced<IConfiguration>::get()->get(&begin, &end);
				if (rc != S_OK) return;


				for (; begin != end; begin++)
					callback(refcnt::make_shared_reference_ext<SFilter_Parameter, IFilter_Parameter>(*begin, true));
			}
		};
	}

	using SFilter_Configuration = internal::CInternal_Filter_Configuration<IFilter_Configuration>;

	class SFilter_Configuration_Link : public virtual internal::CInternal_Filter_Configuration<IFilter_Configuration_Link> {
	public:
		TFilter_Descriptor descriptor();
		SFilter_Parameter Add_Parameter(const scgms::NParameter_Type type, const wchar_t *conf_name);
	};

	namespace internal {

		scgms::SFilter_Configuration_Link Create_Configuration_Link(const GUID &id);

		template <typename IChain_Configuration>
		class CInternal_Filter_Chain_Configuration : public virtual refcnt::SReferenced<IChain_Configuration> {
		public:
			SFilter_Configuration_Link Add_Link(const GUID &id) {
				scgms::SFilter_Configuration_Link link = Create_Configuration_Link(id);				
				if (link) {
					scgms::IFilter_Configuration_Link *raw_link = link.get();
					refcnt::SReferenced<IChain_Configuration>::get()->add(&raw_link, &raw_link + 1);
				}

				return link;
			}


			void for_each(std::function<void(scgms::SFilter_Configuration_Link)> callback) {
				scgms::IFilter_Configuration_Link **link_begin, **link_end;
				HRESULT rc = refcnt::SReferenced<IChain_Configuration>::get()->get(&link_begin, &link_end);
				if (rc != S_OK) return;


				for (; link_begin != link_end; link_begin++)
					callback(refcnt::make_shared_reference_ext<SFilter_Configuration_Link, IFilter_Configuration_Link>(*link_begin, true));
			}


			scgms::SFilter_Configuration_Link operator[](const size_t index) {
				scgms::SFilter_Configuration_Link result;
				scgms::IFilter_Configuration_Link **begin, **end;

		
				if (refcnt::SReferenced<IChain_Configuration>::get()->get(&begin, &end) == S_OK) {
					if (begin + index < end) {	//check the bounds
						result = refcnt::make_shared_reference_ext<scgms::SFilter_Configuration_Link, scgms::IFilter_Configuration_Link>(*(begin + index), true);
					}
				}

				return result;
			}

		};
	}

	using SFilter_Chain_Configuration = internal::CInternal_Filter_Chain_Configuration<scgms::IFilter_Chain_Configuration>;

	class SPersistent_Filter_Chain_Configuration : public virtual internal::CInternal_Filter_Chain_Configuration<scgms::IPersistent_Filter_Chain_Configuration> {
	public:
		SPersistent_Filter_Chain_Configuration();				
	};

	class SFilter_Executor : public virtual refcnt::SReferenced<scgms::IFilter_Executor> {
	public:
		SFilter_Executor() : refcnt::SReferenced<scgms::IFilter_Executor>() {};
		SFilter_Executor(refcnt::SReferenced<scgms::IFilter_Chain_Configuration> configuration, scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data, refcnt::Swstr_list error_description);

		HRESULT Execute(scgms::UDevice_Event &&event);
	};

	using SFilter_Feedback_Receiver = refcnt::SReferenced<scgms::IFilter_Feedback_Receiver>;

	#pragma warning( push )
	#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

	class CBase_Filter : public virtual scgms::IFilter, public refcnt::CReferenced {
	protected:
		scgms::SFilter mOutput;	//aka the next_filter
		HRESULT Send(scgms::UDevice_Event &event);
	protected:
		const GUID mDevice_ID = Invalid_GUID;
		void Emit_Info(const scgms::NDevice_Event_Code code, const std::wstring &msg, const uint64_t segment_id = scgms::Invalid_Segment_Id);
	protected:
		//Descending class is supposed to implement these two methods only
		virtual HRESULT Do_Execute(scgms::UDevice_Event event) = 0;
		virtual HRESULT Do_Configure(scgms::SFilter_Configuration configuration, refcnt::Swstr_list &error_description) = 0;
	public:		
		CBase_Filter(scgms::IFilter* output, const GUID &device_id = Invalid_GUID);
		virtual ~CBase_Filter();
		virtual HRESULT IfaceCalling Configure(IFilter_Configuration* configuration, refcnt::wstr_list* error_description) override final;
		virtual HRESULT IfaceCalling Execute(scgms::IDevice_Event *event) override final;
	};

	#pragma warning( pop )

	class SDiscrete_Model : public virtual refcnt::SReferenced<scgms::IDiscrete_Model> {
	public:
		SDiscrete_Model();
		SDiscrete_Model(const GUID &id, const std::vector<double> &parameters, scgms::SFilter output);
	};

	class SDrawing_Filter_Inspection : public std::shared_ptr<IDrawing_Filter_Inspection> {
	public:
		SDrawing_Filter_Inspection() noexcept {};
		SDrawing_Filter_Inspection(const SFilter &drawing_filter);
	};

	class SLog_Filter_Inspection : public std::shared_ptr<ILog_Filter_Inspection> {
	public:
		SLog_Filter_Inspection() noexcept {};
		SLog_Filter_Inspection(const SFilter &log_filter);
		bool pop(std::shared_ptr<refcnt::wstr_list> &list);
	};

	class SSignal_Error_Inspection : public virtual refcnt::SReferenced<scgms::ISignal_Error_Inspection> {
	public:
		SSignal_Error_Inspection() noexcept {};
		SSignal_Error_Inspection(const SFilter &signal_error_filter);
	};

}


std::wstring Select_Time_Segments_Id_To_WString(scgms::time_segment_id_container *container);
scgms::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(scgms::IModel_Parameter_Vector *container);
scgms::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
