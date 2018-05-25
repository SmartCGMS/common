#include "DbLib.h"

namespace db {

	bool SDb_Query::Bind_Parameters(const std::vector<TParameter> &query_parameters) {
		if (!operator bool()) return false;

		return get()->Bind_Parameters(const_cast<db::TParameter*>(query_parameters.data()), query_parameters.size()) == S_OK;
	}

	bool SDb_Query::Get_Next(std::vector<TParameter> &result) {
		if (!operator bool()) return false;

		return get()->Get_Next(result.data(), result.size()) == S_OK;
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