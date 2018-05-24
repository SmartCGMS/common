#pragma once

#include "../iface/DbIface.h"

#include <vector>
#include <array>
#include <variant>
#include <string_view>


namespace db {

	using TParameter = std::variant<int64_t, double, std::wstring_view, bool>;
	struct TIndexed_Parameter {
		size_t index;		
		TParameter parameter;
	};

	class SDb_Query : public std::shared_ptr<IDb_Query>{
	public:
		bool Bind_Parameters(std::vector<TParameter> query_parameters, std::vector<TIndexed_Parameter> result);
			//set the result.parameter value first to indicate desired parameter type
			//the idea of overtaking the Get_Next parameters is to improve performance by preparing IDb_Query::Get_Next's structures just once
		bool Get_Next();		
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
