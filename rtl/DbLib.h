#pragma once

#include "../iface/DbIface.h"

#include <vector>

namespace db {

	
	class SDb_Query : public std::shared_ptr<IDb_Query>{
	public:
		bool Bind_Parameters(const std::vector<TParameter> &query_parameters);
		bool Get_Next(std::vector<TParameter> &result);
	};

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
