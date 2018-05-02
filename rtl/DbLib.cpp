#include "DbLib.h"
namespace db
{
	
	SDb_Connection SDb_Connector::Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password) {
		IDb_Connection *connection;
		SDb_Connection result;
		if (get()->Connect(host.c_str(), provider.c_str(), port, name.c_str(), user_name.c_str(), password.c_str(), &connection) == S_OK)
			result = refcnt::make_shared_reference_ext<SDb_Connection, IDb_Connection>(obj, false);

		return result;
	}
	
}
