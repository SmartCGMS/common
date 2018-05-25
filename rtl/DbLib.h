#pragma once

#include "../iface/DbIface.h"

#include <vector>

namespace db {

	
	class SDb_Query : public ::std::shared_ptr<IDb_Query>{	
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
				desc.bl = param1;
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

			return Bind_Parameters(param1) && Bind_Parameters(Args);
		}



		bool Get_Next(std::vector<TParameter> &result);
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
