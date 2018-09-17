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

#pragma once

#include "../iface/DbIface.h"
#include "../rtl/referencedImpl.h"

#include <vector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

class CDb_Query : public virtual db::IDb_Query, public virtual refcnt::CReferenced {
protected:
	QSqlQuery mQuery;
	bool mExecuted = false;
	std::vector<std::wstring> mResult_String_Row;
public:
	CDb_Query(QSqlDatabase &db, const wchar_t *statement);
	virtual ~CDb_Query() {};
	virtual HRESULT IfaceCalling Bind_Parameters(const db::TParameter *values, const size_t count) override final;
	virtual HRESULT IfaceCalling Get_Next(db::TParameter* const values, const size_t count) override final;
	virtual HRESULT IfaceCalling Cancel() override final;
};

class CDb_Connection : public virtual db::IDb_Connection, public virtual refcnt::CReferenced {
protected:
	QString mConnection_Name;
public:
	CDb_Connection(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password);
	virtual ~CDb_Connection() {}

	virtual HRESULT IfaceCalling Query(const wchar_t *statement, db::IDb_Query **query) override final;
};

class CDb_Connector : public virtual db::IDb_Connector, public virtual refcnt::CNotReferenced {
public:
	virtual ~CDb_Connector() {}

	virtual HRESULT IfaceCalling Connect(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password, db::IDb_Connection **connection) override final;
};

#pragma warning( pop )

extern CDb_Connector db_connector;