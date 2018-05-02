#pragma once

#include "../iface/DbIface.h"
#include "../rtl/referencedImpl.h"

#include <QtSql/QSqlDatabase>

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

class CDb_Connection : public virtual db::IDb_Connection, public virtual refcnt::CReferenced {
protected:
	QSqlDatabase mDb;
	QString mConnection_Name;
public:
	CDb_Connection(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password);
	virtual ~CDb_Connection() {}

	virtual HRESULT IfaceCalling Get_Raw(void **qdb) override final;
};

class CDb_Connector : public virtual db::IDb_Connector, public virtual refcnt::CNotReferenced {
public:
	virtual ~CDb_Connector() {}

	virtual HRESULT IfaceCalling Connect(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password, db::IDb_Connection **connection) override final;
};

#pragma warning( pop )

extern CDb_Connector db_connector;