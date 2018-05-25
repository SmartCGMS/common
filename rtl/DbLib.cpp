#include "DbLib.h"

namespace db {

	bool SDb_Query::Get_Next() {
		if (!operator bool()) return false;
		if (get()->Get_Next(mRow_Storage.data(), mRow_Storage.size()) == S_OK) {
			for (size_t i = 0; i < mRow_Storage.size(); i++) {
				switch (mRow_Bindings[i].type) {
					case db::NParameter_Type::ptInt64:		
						*(reinterpret_cast<int64_t*>(mRow_Bindings[i].str)) = mRow_Storage[i].type != db::NParameter_Type::ptNull ? mRow_Storage[i].integer : 0;
						break;

					case db::NParameter_Type::ptDouble:		
						*(reinterpret_cast<double*>(mRow_Bindings[i].str)) = mRow_Storage[i].type != db::NParameter_Type::ptNull ? mRow_Storage[i].dbl : std::numeric_limits<double>::quiet_NaN();
						break;

					case db::NParameter_Type::ptWChar:		
						*(reinterpret_cast<wchar_t**>(mRow_Bindings[i].str)) = mRow_Storage[i].type != db::NParameter_Type::ptNull ? mRow_Storage[i].str : nullptr;
						break;

					case db::NParameter_Type::ptBool:
						*(reinterpret_cast<bool*>(mRow_Bindings[i].str)) = mRow_Storage[i].type != db::NParameter_Type::ptNull ? mRow_Storage[i].boolean : false;
						break;

				}
			}
		}
	}

	SDb_Query SDb_Connection::Query(const std::wstring &statement) {
		IDb_Query *query;
		SDb_Query result;
		if ((operator bool()) && (get()->Query(statement.c_str(), &query) == S_OK)) {
			result = refcnt::make_shared_reference_ext<SDb_Query, IDb_Query>(query, false);
		}

		return result;
	}

	
	SDb_Connection SDb_Connector::Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password) {
		IDb_Connection *connection;
		SDb_Connection result;
		if ((operator bool()) && (get()->Connect(host.c_str(), provider.c_str(), port, name.c_str(), user_name.c_str(), password.c_str(), &connection) == S_OK))
			result = refcnt::make_shared_reference_ext<SDb_Connection, IDb_Connection>(connection, false);

		return result;
	}
	
}