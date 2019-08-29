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

	class SFilter_Pipe_Reader : public virtual refcnt::SReferenced<IFilter_Pipe_Reader> {
	public:
		UDevice_Event Receive()  {
			if (!(operator bool())) return UDevice_Event{};

			IDevice_Event *raw_event;			
			if (get()->receive(&raw_event) != S_OK) return nullptr;

			return UDevice_Event{ raw_event };
		}
	};

	class SFilter_Pipe_Writer : public virtual refcnt::SReferenced<IFilter_Pipe_Writer> {
	public:
		bool Send(UDevice_Event &event)  {	//consumes the event in any case
			if (!(operator bool())) return false;
			if (!event) return false;

			if (get()->send(event.get()) != S_OK) {
				event.reset(nullptr);	//delete and release the event anyway to prevent the event from being deleted twice
				return false;
			}

			event.release(); 	//release the resource from the sender, but do not delete it to let it live in the next filter

			return true;
		}
	};

	using SFilter = refcnt::SReferenced<IFilter>;

	class CFilter_Pipe {
	public:
		CFilter_Pipe() {}
		virtual ~CFilter_Pipe() {}

		virtual bool Send(UDevice_Event &event) = 0;	//consumes the event in any case
		virtual UDevice_Event Receive() = 0;
		virtual bool add_filter(SFilter &filter) = 0;
		virtual bool empty() const = 0;

		virtual glucose::IFilter_Pipe_Reader* get_async_reader() = 0;
		virtual glucose::IFilter_Pipe_Writer* get_async_writer() = 0;

		virtual glucose::IFilter_Pipe_Reader* get_sync_reader() = 0;
		virtual glucose::IFilter_Pipe_Writer* get_sync_writer() = 0;
	};

	template <typename I>
	class SFilter_Pipe : public virtual CFilter_Pipe, public std::shared_ptr<I>{
	public:
		SFilter_Pipe(I *pipe) {
			if (pipe) pipe->AddRef();
			reset(pipe, [](I* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
		}

		SFilter_Pipe() : CFilter_Pipe() {}

		virtual ~SFilter_Pipe() {}
		

		virtual bool Send(UDevice_Event &event) final {	//consumes the event in any case
			if (!(std::shared_ptr<I>::operator bool())) return false;
			if (!event) return false;

			if (std::shared_ptr<I>::get()->send(event.get()) != S_OK) {
				event.reset(nullptr);	//delete and release the event anyway to prevent the event from being deleted twice
				return false;
			}

			event.release(); 	//release the resource from the sender, but do not delete it to let it live in the next filter

			return true;
		}

		virtual UDevice_Event Receive() final {
			if (!(std::shared_ptr<I>::operator bool())) return UDevice_Event{};

			IDevice_Event *raw_event;
			if (std::shared_ptr<I>::get()->receive(&raw_event) != S_OK) return nullptr;

			return UDevice_Event{ raw_event };
		}

		virtual glucose::IFilter_Asynchronous_Pipe* get_raw_pipe() final {					
			return static_cast<glucose::IFilter_Asynchronous_Pipe*>(std::shared_ptr<I>::get());
		}

		virtual bool empty() const final {
			return !(std::shared_ptr<I>::operator bool());
		}
	};

	class SFilter_Asynchronous_Pipe : public virtual SFilter_Pipe<glucose::IFilter_Asynchronous_Pipe> {
	public:
		SFilter_Asynchronous_Pipe();				
		virtual bool add_filter(SFilter &filter) final;

		virtual glucose::IFilter_Pipe_Reader* get_async_reader() { return static_cast<glucose::IFilter_Pipe_Reader*>(get()); }
		virtual glucose::IFilter_Pipe_Writer* get_async_writer() { return static_cast<glucose::IFilter_Pipe_Writer*>(get()); }

		virtual glucose::IFilter_Pipe_Reader* get_sync_reader() { return nullptr; }
		virtual glucose::IFilter_Pipe_Writer* get_sync_writer() { return nullptr; }
	};

	class SFilter_Synchronous_Pipe : public virtual SFilter_Pipe<glucose::IFilter_Synchronous_Pipe> {
	public:
		SFilter_Synchronous_Pipe();
		virtual bool add_filter(SFilter &filter) final;

		virtual glucose::IFilter_Pipe_Reader* get_async_reader() { return static_cast<glucose::IFilter_Pipe_Reader*>(get()); }
		virtual glucose::IFilter_Pipe_Writer* get_async_writer() { return static_cast<glucose::IFilter_Pipe_Writer*>(get()); }

		virtual glucose::IFilter_Pipe_Reader* get_sync_reader() { return get()->Get_Reader(); }
		virtual glucose::IFilter_Pipe_Writer* get_sync_writer() { return get()->Get_Writer(); }
	};
	
	

	bool add_filters(const std::vector<glucose::TFilter_Descriptor> &descriptors, glucose::TCreate_Filter create_filter);

	std::vector<TFilter_Descriptor> get_filter_descriptors();
	bool get_filter_descriptor_by_id(const GUID &id, TFilter_Descriptor &desc);

	SFilter create_filter(const GUID &id, IFilter_Pipe_Reader *input, IFilter_Pipe_Writer *output);

	class SFilter_Parameters : public std::shared_ptr<glucose::IFilter_Configuration> {
	public:
		std::wstring Read_String(const wchar_t* name, const std::wstring& default_value = {}) const;
		int64_t Read_Int(const wchar_t* name, const int64_t default_value = std::numeric_limits<int64_t>::max()) const;
		std::vector<int64_t> Read_Int_Array(const wchar_t* name) const;
		GUID Read_GUID(const wchar_t* name, const GUID &default_value = Invalid_GUID) const;
		bool Read_Bool(const wchar_t* name, bool default_value = false) const;
		double Read_Double(const wchar_t* name, const double default_value = std::numeric_limits<double>::quiet_NaN()) const;
		void Read_Parameters(const wchar_t* name, glucose::SModel_Parameter_Vector &lower_bound, glucose::SModel_Parameter_Vector &default_parameters, glucose::SModel_Parameter_Vector &upper_bound) const;

		std::vector<double> Read_Double_Array(const wchar_t* name) const;	//TODO: remove in the future in favor to Read_Parameters

		TFilter_Parameter* Resolve_Parameter(const wchar_t* name) const;
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


	/* this will vanish
	class SDevice_Event_Vector : public std::shared_ptr<glucose::IDevice_Event_Vector> {
	private:
		std::vector<glucose::IDevice_Event*> mAddedEvents;

	public:
		SDevice_Event_Vector();
		virtual ~SDevice_Event_Vector();
		SDevice_Event_Vector& operator=(const SDevice_Event_Vector&) = default;

		// adds event to wrapped container
		bool push(glucose::UDevice_Event& evt); - bylo add, proc mit dve varianty?
		// adds event to internal vector; this required Apply or Discard to be called
		bool push_deferred(glucose::UDevice_Event& evt);

		bool pop(glucose::UDevice_Event &evt);

		bool commit_push();
		bool discard_push();
	};
	*/
}


std::wstring Select_Time_Segments_Id_To_WString(glucose::time_segment_id_container *container);
glucose::time_segment_id_container* WString_To_Select_Time_Segments_Id(const wchar_t *str);
std::wstring Model_Parameters_To_WString(glucose::IModel_Parameter_Vector *container);
glucose::IModel_Parameter_Vector* WString_To_Model_Parameters(const wchar_t *str);
