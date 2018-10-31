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

	
	class SFilter_Pipe : public std::shared_ptr<IFilter_Pipe> {
	public:
		SFilter_Pipe(glucose::IFilter_Pipe *pipe);
		SFilter_Pipe();
		bool Send(UDevice_Event &event);	//consumes the event in any case
		UDevice_Event Receive();
	};


	using SFilter = std::shared_ptr<IFilter>;

	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);
	
	SFilter create_filter(const GUID &id, SFilter_Pipe &input, SFilter_Pipe &output);

	class SFilter_Parameters : public std::shared_ptr<glucose::IFilter_Configuration> {
	protected:
		TFilter_Parameter* Resolve_Parameter(const wchar_t* name);
	public:
		std::wstring Read_String(const wchar_t* name);
		int64_t Read_Int(const wchar_t* name, const int64_t default_value = std::numeric_limits<int64_t>::max());
		std::vector<int64_t> Read_Int_Array(const wchar_t* name);
		GUID Read_GUID(const wchar_t* name, const GUID &default_value = Invalid_GUID);
		bool Read_Bool(const wchar_t* name, bool default_value = false);
		double Read_Double(const wchar_t* name);
		void Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound);
		
		std::vector<double> Read_Double_Array(const wchar_t* name);	//TODO: remove in the future in favor to Read_Parameters
	};


	void Visit_Filter_Parameter(glucose::TFilter_Parameter& element, std::function<void(refcnt::IReferenced *obj)> func);
	void Release_Filter_Parameter(TFilter_Parameter &parameter);

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
