/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#include "DbLib.h"

namespace db {

	bool SDb_Query::Get_Next() {

		if (!operator bool())
			return false;

		//as the row storage data type could have been altered due to null values, we must reinitialize it
		mRow_Storage.resize(mRow_Bindings.size());
		for (size_t i = 0; i < mRow_Bindings.size(); i++)
			mRow_Storage[i].type = mRow_Bindings[i].type;

		HRESULT query_rc = get()->Get_Next(mRow_Storage.data(), mRow_Storage.size());
		if (query_rc == S_OK) {
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

					default:
						break;
				}
			}
		}
		else 
			return mRow_Bindings.empty() ?  query_rc == S_FALSE : false;	//SQL statement, e.g., insert does not need to return anything, yet it succeeds with S_FALSE code

		return true;
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