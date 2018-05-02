#pragma once

#include "../iface/DbIface.h"

namespace db
{
	using SDb_Connection = std::shared_ptr<db::IDb_Connection>;

	class SDb_Connector : public std::shared_ptr<db::IDb_Connector> {
	public:
		SDb_Connection Connect(const std::wstring &host, const std::wstring &provider, uint16_t port, const std::wstring &name, const std::wstring &user_name, const std::wstring &password);
	};
}
