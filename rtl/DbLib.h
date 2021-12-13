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

#include "../iface/DbIface.h"

#include <vector>
#include <string>

namespace db {

	
	class SDb_Query : public ::std::shared_ptr<IDb_Query>{
	protected:
		std::vector<TParameter> mRow_Storage;
		// stores original, required type and the pointer
		std::vector<TParameter> mRow_Bindings;

		bool mReady_To_Clear_Result_Bindings = true;
		template <typename... Args>
		void Clear_Result_Bindings(Args...) {
			if (mReady_To_Clear_Result_Bindings) {
				mRow_Bindings.clear();
				mReady_To_Clear_Result_Bindings = false;
			}
		}
	public:
		template <typename TParam1, typename ...Args>
		bool Bind_Parameters(TParam1 param1, Args... args) {
			return Bind_Parameters(param1) && Bind_Parameters(args...);
		}

		template <typename TParam1>
		bool Bind_Parameters(TParam1 param1) {
			if (!operator bool()) return false;

			TParameter desc{};
			if (!Bind_Parameter(param1, desc))
				return false;

			return get()->Bind_Parameters(&desc, 1) == S_OK;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_integral_v<TParam1> && !std::is_same_v<std::remove_const_t<TParam1>, bool>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter &desc) {
			desc.type = db::NParameter_Type::ptInt64;
			desc.integer = static_cast<decltype(desc.integer)>(param1);
			return true;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_same_v<std::remove_const_t<TParam1>, double>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter &desc) {
			desc.type = db::NParameter_Type::ptDouble;
			desc.dbl = static_cast<decltype(desc.dbl)>(param1);
			return true;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_same_v<std::remove_const_t<TParam1>, bool>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter &desc) {
			desc.type = db::NParameter_Type::ptBool;
			desc.boolean = static_cast<decltype(desc.boolean)>(param1 ? TRUE : FALSE);
			return true;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_same_v<std::remove_const_t<TParam1>, wchar_t*> || std::is_same_v<std::remove_const_t<TParam1>, const wchar_t*>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter &desc) {
			desc.type = db::NParameter_Type::ptWChar;
			desc.str = reinterpret_cast<decltype(desc.str)>(const_cast<decltype(desc.str)>(param1));
			return true;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_same_v<std::remove_const_t<TParam1>, GUID>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter& desc) {
			desc.type = db::NParameter_Type::ptGuid;
			desc.id = static_cast<decltype(desc.id)>(param1);
			return true;
		}

		template <typename TParam1, typename std::enable_if_t<std::is_same_v<std::remove_const_t<TParam1>, TBinary_Object>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter& desc) {
			desc.type = db::NParameter_Type::ptBinaryObect;
			desc.binary_object = static_cast<decltype(desc.binary_object)>(param1);
			return true;
		}

		template <typename TParam1 = std::nullptr_t, typename std::enable_if_t<std::is_null_pointer_v<TParam1>, TParam1>* = nullptr>
		bool Bind_Parameter(TParam1 param1, TParameter &desc) {
			desc.type = db::NParameter_Type::ptNull;
			return true;
		}



		template <typename TParam1, typename ...Args>
		bool Bind_Result(TParam1 &param1, Args&... args) {				//binds particular variables for a repeated call of Get_Next
			return Bind_Result(param1) && Bind_Result(args...);
		}

		template <typename TParam1>
		bool Bind_Result(TParam1 &param1) {
			if (!operator bool()) return false;

			TParameter desc;
			if (std::is_same<TParam1, int64_t>::value)  desc.type = db::NParameter_Type::ptInt64;
			else if (std::is_same<TParam1, double>::value) desc.type = db::NParameter_Type::ptDouble;
			else if (std::is_same<TParam1, bool>::value) desc.type = db::NParameter_Type::ptBool;
			else if (std::is_same<TParam1, wchar_t*>::value) desc.type = db::NParameter_Type::ptWChar;
			else if (std::is_same<TParam1, GUID>::value) desc.type = db::NParameter_Type::ptGuid;
			else if (std::is_same<TParam1, TBinary_Object>::value) desc.type = db::NParameter_Type::ptBinaryObect;
			else return false;
			
			desc.str = reinterpret_cast<wchar_t*>(&param1);	//intentionally missusing wchar_t* as void* (pointer as a pointer;)
			mRow_Bindings.push_back(desc);

			return true;
		}

		bool Bind_Result(std::vector<double> &param) {
			if (!operator bool()) return false;
			for (size_t i = 0; i < param.size(); i++) {
				db::TParameter desc;
				desc.type = db::NParameter_Type::ptDouble;
				desc.str = reinterpret_cast<wchar_t*>(&param[i]);	//intentionally missusing wchar_t* as void* (pointer as a pointer;)
				mRow_Bindings.push_back(desc);
			}

			return true;
		}

		template <typename TParam1, typename ...Args>
		bool Get_Next(TParam1 &param1, Args&... args) {		//designed for one-time call of Get_Next, which reads one row into particular variables
			if (!operator bool()) return false;

			Clear_Result_Bindings(param1, args...);
			if (!Bind_Result(param1, args...)) return false;
			mReady_To_Clear_Result_Bindings = true;

			return Get_Next();
		}

		template <typename TParam1>
		bool Get_Next(TParam1 &param1) {
			if (!operator bool()) return false;
			Clear_Result_Bindings(param1);

			if (!Bind_Result(param1)) return false;
			mReady_To_Clear_Result_Bindings = true;

			return Get_Next();
		}

		bool Get_Next();

		bool Execute() {		//just a shortcut with a name that indicates the intention to execute query that returns no result set
			mRow_Bindings.clear();
			return Get_Next();
		}
	};

	class SDb_Connection : public std::shared_ptr<IDb_Connection> {
	public:
		SDb_Query Query(const std::wstring &statement);

		template <typename... Args>
		SDb_Query Query(const std::wstring &statement, Args... args)  {
			SDb_Query result = Query(statement);
			if (!result.Bind_Parameters(args...)) result.reset();
			return result;
		}

	};

	class SDb_Connector : public std::shared_ptr<IDb_Connector> {
	public:
		SDb_Connection Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password);
	};

	using SDb_Sink = std::shared_ptr<IDb_Sink>;

	bool is_file_db(const std::wstring& provider);
}
