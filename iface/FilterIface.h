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

#include <memory>
#include <string>
#include <vector>

#include "DeviceIface.h"
#include "referencedIface.h"
#include "../utils/winapi_mapping.h"

namespace scgms {

	using time_segment_id_container = refcnt::IVector_Container<int64_t>;

	enum class NParameter_Type : size_t {
		ptNull = 0,
		ptWChar_Array,			//IParameter_Container<wchar_t>
		ptInt64_Array,	//alias for IParameter_Container<int64_t> that selects time segments id
		ptDouble,	
		ptRatTime,					//double interperted as the rattime
		ptInt64,
		ptBool,
		ptSignal_Model_Id,
		ptDiscrete_Model_Id,
		ptMetric_Id,
		ptSolver_Id,
		ptModel_Produced_Signal_Id,	// signal dependend on model selection
		ptSignal_Id,		// any signal available (measured, calculated)
		ptDouble_Array,		// three parameter sets in one container - lower bound, default values, higher bound
		ptSubject_Id		// int64_t, but with additional functionality in GUI
	}; 

	class IFilter_Parameter : public virtual refcnt::IReferenced {
	public:
		//read-only
		virtual HRESULT IfaceCalling Get_Type(NParameter_Type *type) = 0;
		virtual HRESULT IfaceCalling Get_Config_Name(wchar_t **config_name) = 0;	
		
		//read-write
		virtual HRESULT IfaceCalling Get_WChar_Container(refcnt::wstr_container **wstr, BOOL read_interpreted) = 0;	//$(Variable_Name) reads system-variable with read_interpreted==true
		virtual HRESULT IfaceCalling Set_WChar_Container(refcnt::wstr_container *wstr) = 0;	

		virtual HRESULT IfaceCalling Get_Time_Segment_Id_Container(time_segment_id_container **ids) = 0;
		virtual HRESULT IfaceCalling Set_Time_Segment_Id_Container(time_segment_id_container *ids) = 0;

		virtual HRESULT IfaceCalling Get_Double(double *value) = 0;
		virtual HRESULT IfaceCalling Set_Double(const double value) = 0;

		virtual HRESULT IfaceCalling Get_Int64(int64_t *value) = 0;
		virtual HRESULT IfaceCalling Set_Int64(const int64_t value) = 0;

		virtual HRESULT IfaceCalling Get_Bool(BOOL *boolean) = 0;
		virtual HRESULT IfaceCalling Set_Bool(const BOOL boolean) = 0;

		virtual HRESULT IfaceCalling Get_GUID(GUID *id) = 0;
		virtual HRESULT IfaceCalling Set_GUID(const GUID *id) = 0;

		virtual HRESULT IfaceCalling Get_Model_Parameters(scgms::IModel_Parameter_Vector **parameters) = 0;
		virtual HRESULT IfaceCalling Set_Model_Parameters(scgms::IModel_Parameter_Vector *parameters) = 0;
	};
	

	using IFilter_Configuration = refcnt::IVector_Container<scgms::IFilter_Parameter*>;
	
	class IFilter_Configuration_Link : public virtual IFilter_Configuration {
	public:
		virtual HRESULT IfaceCalling Get_Filter_Id(GUID *id) = 0;		
	};

	using IFilter_Chain_Configuration = refcnt::IVector_Container<IFilter_Configuration_Link*>;

	class IPersistent_Filter_Chain_Configuration : public virtual IFilter_Chain_Configuration {
	public:
			//both Load_From_ methods returns S_FALSE if incomplete configuration was constructed
		virtual HRESULT IfaceCalling Load_From_File(const wchar_t *file_path, refcnt::wstr_list* error_description) = 0;	//if nullptr, assumes default config file name
		virtual HRESULT IfaceCalling Load_From_Memory(const char *memory, const size_t len, refcnt::wstr_list* error_description) = 0;
		virtual HRESULT IfaceCalling Save_To_File(const wchar_t *file_path) = 0; //if nullptr, saves to the file_name previously supplied to Load_From_File		
	};	

	class IFilter : public virtual refcnt::IReferenced {
	public:
		//Set up filter configuration, possibly during its execution
		virtual HRESULT IfaceCalling Configure(IFilter_Configuration* configuration, refcnt::wstr_list *error_description) = 0;

		//Executes the filter's control loop
		//when called, the filter owns the event - the filter has to either forward the event, 
		//or call Release on it to discard it
		//Filter forwards existing/sends new event using IFilter_Communicator * supplied to its constructor
		virtual HRESULT IfaceCalling Execute(scgms::IDevice_Event *event) = 0;	
	};

	class IFilter_Executor : public virtual refcnt::IReferenced {	//IEvent_Sender sends the event to the first filter
	public:
			//whatever Filter_Executor swallows it never releases back => Execute always consumes the event
		virtual HRESULT IfaceCalling Execute(scgms::IDevice_Event *event) = 0;
		virtual HRESULT IfaceCalling Wait_For_Shutdown_and_Terminate() = 0;	//returns once all filters have terminated and joined
		virtual HRESULT IfaceCalling Terminate() = 0;	//returns once all filters are executing		
	};
	
	class IFilter_Feedback : public virtual scgms::IFilter {
	public:
		virtual HRESULT IfaceCalling Name(wchar_t** const name) = 0;
	};

	constexpr GUID IID_Filter_Feedback_Receiver = { 0xee9d9028, 0xb714, 0x4412, { 0x98, 0xd8, 0xe5, 0xf7, 0xe5, 0xf1, 0xcf, 0x7a } };
	class IFilter_Feedback_Receiver : public virtual scgms::IFilter_Feedback {
	};

	constexpr GUID IID_Filter_Feedback_Sender = { 0x19d21259, 0x7358, 0x4533, { 0x8c, 0x18, 0x85, 0x65, 0x25, 0xc2, 0x35, 0x9 } };
	class IFilter_Feedback_Sender : public scgms::IFilter_Feedback {
	public:
		virtual HRESULT IfaceCalling Sink(scgms::IFilter_Feedback_Receiver *receiver) = 0;
	};


	class IDiscrete_Model : public virtual scgms::IFilter {
	public:
		virtual HRESULT IfaceCalling Initialize(const double current_time, const uint64_t segment_id) = 0;
		virtual HRESULT IfaceCalling Step(const double time_advance_delta) = 0;	//time_advance_delta == 0.0 means to emit the current state
	};

	using TCreate_Persistent_Filter_Chain_Configuration = HRESULT(IfaceCalling *)(IPersistent_Filter_Chain_Configuration **configuration);
	using TCreate_Filter = HRESULT(IfaceCalling *)(const GUID *id, IFilter *next_filter, scgms::IFilter **filter);
	using TOn_Filter_Created = HRESULT(IfaceCalling *)(scgms::IFilter *filter, const void* data);
	using TExecute_Filter_Configuration = HRESULT(IfaceCalling*)(IFilter_Chain_Configuration *configuration, scgms::TOn_Filter_Created on_filter_created, const void* on_filter_created_data, scgms::IFilter_Executor **executor, refcnt::wstr_list *error_description);
	using TCreate_Filter_Parameter = HRESULT(IfaceCalling*)(const scgms::NParameter_Type type, const wchar_t *config_name, scgms::IFilter_Parameter **parameter);
	using TCreate_Filter_Configuration_Link = HRESULT(IfaceCalling*)(const GUID *filter_id, scgms::IFilter_Configuration_Link **link);
	using TCreate_Discrete_Model = HRESULT(IfaceCalling*)(const GUID *model_id, scgms::IModel_Parameter_Vector *parameters, scgms::IFilter *output, scgms::IDiscrete_Model **model);

	//The following GUIDs advertise known filters 		
	constexpr GUID IID_Drawing_Filter = { 0x850a122c, 0x8943, 0xa211,{ 0xc5, 0x14, 0x25, 0xba, 0xa9, 0x14, 0x35, 0x74 } };
	constexpr GUID IID_Log_Filter = { 0xc0e942b9, 0x3928, 0x4b81,{ 0x9b, 0x43, 0xa3, 0x47, 0x66, 0x82, 0x0, 0xBA } };

	//The following interfaces can be access via refcnt::IUnknown::QueryInterface 


	enum class NError_Marker : size_t {
		Average = 0,
		StdDev,
		AIC,
		Sum,
		count
	};

	enum class NError_Percentile : size_t {
		Minimum = 0,
		P25,
		Median,
		P75,
		P95,
		P99,
		Maximum,
		count
	};

	enum class NError_Range : size_t {
		R5,
		R10,
		R25,
		R50,
		count
	};

	// Structure for containing error metric values
	struct TError_Markers {
		union
		{
			double markers[static_cast<size_t>(NError_Marker::count)];
			struct
			{
				double avg;						// average value
				double stddev;					// standard deviation with Bessel's correction
				double aic;						// Akaike's information criterion
				double sum;						// sum of errors
			};
		};
		union
		{
			double percentile[static_cast<size_t>(NError_Percentile::count)]; // min, q1, median, q3, q95, q99, max
			struct
			{
				double minval;					// minimum value (0. percentile)
				double p25;						// 1. quartile (25. percentile)
				double median;					// median (50. percentile)
				double p75;						// 3. quartile (75. percentile)
				double p95;						// 95. percentile
				double p99;						// 99. percentile
				double maxval;					// maximum value ("100." percentile)
			};
		};
		union
		{
			double range[static_cast<size_t>(NError_Range::count)];
			struct
			{
				double r5;						// 5% range
				double r10;						// 10% range
				double r25;						// 25% range
				double r50;						// 50% range
			};
		};
	};
	
	enum class NECDF : size_t {
		min_value = 0,
		p25 = 24,
		median = 49,
		p75 = 74,
		p95 = 94,
		p99 = 98,
		max_value = 99
	};

	struct TSignal_Stats {
		double avg, stddev, sum;	//standard deviation with Bessel's correction
		size_t count;				//number of elements on which we calculate the metrics
		std::array<double, static_cast<size_t>(NECDF::max_value) + 1> ecdf;
	};

	// error types
	enum class NError_Type : size_t
	{
		Absolute = 0,
		Relative,
		count
	};

	// supported types of drawing outputs
	enum class TDrawing_Image_Type : size_t
	{
		Graph = 0,
		Day,
		Parkes,
		Clark,
		AGP,
		ECDF,
		Profile_Glucose,
		Profile_Insulin,
		Profile_Carbs,

		count
	};

	// supported diagnosis
	enum class TDiagnosis : size_t
	{
		Type1 = 0,
		Type2,
		Gestational,

		count,
		NotSpecified = Type1
	};

	constexpr GUID IID_Signal_Error_Inspection = { 0xfb51bcab, 0x5c2b, 0x45af, { 0x98, 0x80, 0xe3, 0x4d, 0xde, 0xc4, 0x3c, 0x4c } };
	class ISignal_Error_Inspection : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Promise_Metric(const uint64_t segment_id, double* const metric_value, BOOL defer_to_dtor) = 0;
		//return S_OK if there are new data available since object construction or last call of Peek_New_Data_Available
		virtual HRESULT IfaceCalling Peek_New_Data_Available() = 0;
		virtual HRESULT IfaceCalling Calculate_Signal_Error(const uint64_t segment_id, scgms::TSignal_Stats *absolute_error, scgms::TSignal_Stats *relative_error) = 0;
			//should there be a zero reference level, then absolute_error.count != relative_error.count
		virtual HRESULT IfaceCalling Get_Description(wchar_t** const desc) = 0;
	};

	constexpr GUID IID_Drawing_Filter_Inspection = { 0xd0c81596, 0xdea0, 0x4edf,{ 0x8b, 0x97, 0xe1, 0xd3, 0x78, 0xda, 0xfe, 0x3d } };
	class IDrawing_Filter_Inspection : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling New_Data_Available() = 0;
		// retrieves generated SVG for given drawing type and diagnosis		
		virtual HRESULT IfaceCalling Draw(TDrawing_Image_Type type, TDiagnosis diagnosis, refcnt::str_container *svg, refcnt::IVector_Container<uint64_t> *segmentIds, refcnt::IVector_Container<GUID> *signalIds) = 0;
	};

	constexpr GUID IID_Log_Filter_Inspection = { 0xa6054c8d, 0x5c01, 0x9e1d,{ 0x14, 0x39, 0x50, 0xda, 0xd1, 0x08, 0xc9, 0x48 } };
	class ILog_Filter_Inspection : public virtual refcnt::IReferenced {
	public:
		/* retrives newly available log records - caller TAKES ownership of the records
		   returns S_OK if there was at least one log record, S_FALSE if none, E_FAIL otherwise
		*/
		virtual HRESULT IfaceCalling Pop(refcnt::wstr_list **str) = 0;
	};
}
