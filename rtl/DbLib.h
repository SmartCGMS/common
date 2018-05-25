#pragma once

#include "../iface/DbIface.h"

#include <vector>

namespace db {

	
	class SDb_Query : public ::std::shared_ptr<IDb_Query>{	
	protected:
		std::vector<TParameter> mRow_Storage, mRow_Bindings;		
			//mRow_Bindings stores original, required type and the pointer

		bool mReady_To_Clear_Result_Bindings = true;
		template <typename... Args>
		void Clear_Result_Bindings(Args...) {
			if (mReady_To_Clear_Result_Bindings) {
				mRow_Storage.resize(sizeof...(Args)); //only allocating a storage space
				mRow_Bindings.clear();
				mReady_To_Clear_Result_Bindings = false;
			}
		}
	public:
		template <typename TParam1, typename ...Args>
		bool Bind_Parameters(TParam1 param1, Args...) {
			return Bind_Parameters(param1) && Bind_Parameters(Args);
		}

		template <typename TParam1>
		bool Bind_Parameters(TParam1 param1) {
			if (!operator bool()) return false;

			TParameter desc;
			if (std::is_same<TParam1, int64_t>::value) {				
				desc.type = db::NParameter_Type::ptInt64;
				desc.integer = param1;
				
			} else if (std::is_same<TParam1, double>::value) {
				desc.type = db::NParameter_Type::ptDouble;
				desc.dbl = param1;
			}
			else if (std::is_same<TParam1, bool>::value) {
				desc.type = db::NParameter_Type::ptBool;
				desc.boolean = param1;
			}
			else if (std::is_same<TParam1, wchar_t*>::value) {
				desc.type = db::NParameter_Type::ptWChar;
				desc.str = param1;
			}
			else if (std::is_same<TParam1, std::nullptr_t>::value) {
				desc.type = db::NParameter_Type::ptNull;
			}
			else
				return false;

			return get()->Bind_Parameters(&desc, 1) == S_OK;
		}


		template <typename TParam1, typename ...Args>
		bool Bind_Result(TParam1 &param1, Args...) {				//binds particular variables for a repeated call of Get_Next
			return Bind_Result(param1) && Bind_Result(Args...);
		}

		template <typename TParam1>
		bool Bind_Result(TParam1 &param1) {
			if (!operator bool()) return false;

			TParameter desc;
			if (std::is_same<TParam1, int64_t>::value)  desc.type = db::NParameter_Type::ptInt64;						
			else if (std::is_same<TParam1, double>::value) desc.type = db::NParameter_Type::ptDouble;				
			else if (std::is_same<TParam1, bool>::value) desc.type = db::NParameter_Type::ptBool;				
			else if (std::is_same<TParam1, wchar_t*>::value) desc.type = db::NParameter_Type::ptWChar;							
			else return false;
			
			desc.str = static_cast<wchar_t*>(&param1);	//intentionally missusing wchar_t* as void* (pointer as a pointer;)
			mRow_Bindings.push_back(desc);

			return true;
		}

		template <typename TParam1, typename ...Args>
		bool Get_Next(TParam1 &param1, Args...) {		//designed for one-time call of Get_Next, which reads one row into particular variables
			if (!operator bool()) return false;

			Clear_Result_Bindings(param1, Args);
			if (!Bind_Result(param1, Args...)) return false;
			mReady_To_Clear_Result_Bindings = true;

			return Get_Next();
		}

		template <typename TParam1>
		bool Get_Next(TParam1 &param1) {
			if (!operator bool()) return false;
			Clear_Result_Bindings(param1, Args);

			if (!Bind_Result(param1)) return false;
			mReady_To_Clear_Result_Bindings = true;

			return Get_Next();
		}

		bool Get_Next();
	};

	class SDb_Connection : public std::shared_ptr<IDb_Connection> {
	public:
		SDb_Query Query(const std::wstring &statement);

		template <typename ...Args>
		SDb_Query Query(const std::wstring &statement, Args...)  {
			SDb_Query result = Query(statement);
			if (!result.Bind_Parameters(Args...)) result.reset();
			return result;
		}

	};

	class SDb_Connector : public std::shared_ptr<IDb_Connector> {
	public:
		SDb_Connection Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password);
	};

	using SDb_Sink = std::shared_ptr<IDb_Sink>;
}
