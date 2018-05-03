#pragma once

#include "../iface/DbIface.h"

#include <string>
#include <QtSql\QSqlQuery>


namespace db {

	using SDb_Query = std::shared_ptr<IDb_Query>;

	class SDb_Connection : public std::shared_ptr<IDb_Connection> {
	public:
		SDb_Query Query(const std::wstring &statement);
	};

	class SDb_Connector : public std::shared_ptr<IDb_Connector> {
	public:
		SDb_Connection Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password);
	};

	using SDb_Sink = std::shared_ptr<IDb_Sink>;
}
