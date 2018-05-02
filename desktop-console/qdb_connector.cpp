#include "qdb_connector.h"

#include "../rtl/manufactory.h"
#include <QtCore\QUUID>


CDb_Connector db_connector{};


CDb_Connection::CDb_Connection(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password) {
	mConnection_Name = "QDB_Connection_" + QUuid::createUuid().toString();
	mDb = QSqlDatabase::addDatabase(QString::fromWCharArray(provider), mConnection_Name);
	mDb.setHostName(QString::fromWCharArray(host));
	if (port != 0) mDb.setPort(port);
	mDb.setDatabaseName(QString::fromWCharArray(name));
	mDb.setUserName(QString::fromWCharArray(user_name));
	mDb.setPassword(QString::fromWCharArray(password));

}

HRESULT IfaceCalling CDb_Connection::Get_Raw(void **qdb) {
	*reinterpret_cast<QSqlDatabase**>(qdb) = &mDb;
	return S_OK;
}

HRESULT IfaceCalling CDb_Connector::Connect(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password, db::IDb_Connection **connection) {
	return Manufacture_Object<CDb_Connection, db::IDb_Connection>(connection, host, provider, port, name, user_name, password);
}