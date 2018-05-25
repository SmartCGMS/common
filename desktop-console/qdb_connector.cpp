#include "qdb_connector.h"

#include "../rtl/manufactory.h"
#include <QtCore\QUUID>
#include <QtCore\QVariant>

#undef max


CDb_Connector db_connector{};



CDb_Query::CDb_Query(QSqlDatabase &db, const wchar_t *statement) : mQuery(QSqlQuery{ db }) {
	if (!mQuery.prepare(QString::fromWCharArray(statement))) throw std::invalid_argument{ "Malformed SQL statement."};
}


HRESULT IfaceCalling CDb_Query::Bind_Parameters(const db::TParameter *values, const size_t count) {

	for (size_t i = 0; i < count; i++) {		

		switch (values[i].type) {		
			case db::NParameter_Type::ptNull:		mQuery.addBindValue(QVariant{});
													break;

			case db::NParameter_Type::ptInt64:		mQuery.addBindValue(values[i].integer);				
													break;

			case db::NParameter_Type::ptDouble:		mQuery.addBindValue(values[i].dbl);
													break;
			
			case db::NParameter_Type::ptWChar:		mQuery.addBindValue( QString::fromWCharArray(values[i].str));
													break;

			case db::NParameter_Type::ptBool:mQuery.addBindValue(values[i].bl);
													break;
		}
		
	}

	return S_OK;
}

HRESULT IfaceCalling CDb_Query::Get_Next(db::TParameter* const values, const size_t count) {
	if (!mExecuted) mExecuted = mQuery.exec();
	if (!mExecuted) return E_FAIL;

	if (mQuery.next()) {
		mResult_String_Row.resize(std::max(mResult_String_Row.size(), count));

		for (size_t i = 0; i < count; i++) {			
			QVariant db_value = mQuery.value(static_cast<int>(i));
			//first, we had to attempt to read the value as some db drivers
			//will provide valid IsNull() only after that
			if (!mQuery.isNull(static_cast<int>(i))) {

				bool ok_test;
				QString tmp_str;
				

				switch (values[i].type) {
					case db::NParameter_Type::ptInt64:		values[i].integer = static_cast<int64_t>(db_value.toLongLong(&ok_test));
															if (!ok_test) values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptDouble:		values[i].dbl = db_value.toDouble(&ok_test);
															if (!ok_test) values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptWChar:		tmp_str = db_value.toString();
															if (tmp_str.count() > 0) {
																mResult_String_Row[i] = tmp_str.toStdWString();
																values[i].str = const_cast<wchar_t*>(mResult_String_Row[i].c_str());
															} else
																values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptBool:		values[i].bl = db_value.toBool();
															break;
				}
			}
			else
				values[i].type = db::NParameter_Type::ptNull;
		}

		return S_OK;
	}
	else
		return S_FALSE;
}


HRESULT IfaceCalling CDb_Query::Cancel() {
	mQuery.finish();
	return S_OK;
}


CDb_Connection::CDb_Connection(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password) {
	mConnection_Name = "QDB_Connection_" + QUuid::createUuid().toString();
	mDb = QSqlDatabase::addDatabase(QString::fromWCharArray(provider), mConnection_Name);
	mDb.setHostName(QString::fromWCharArray(host));
	if (port != 0) mDb.setPort(port);
	mDb.setDatabaseName(QString::fromWCharArray(name));
	mDb.setUserName(QString::fromWCharArray(user_name));
	mDb.setPassword(QString::fromWCharArray(password));

	if (!mDb.open()) throw "Cannot open database";

}


HRESULT IfaceCalling CDb_Connection::Query(const wchar_t *statement, db::IDb_Query **query) {
	try {
		return Manufacture_Object<CDb_Query, db::IDb_Query>(query, mDb, statement);
	}
	catch (...) {
		return E_FAIL;
	}
}

HRESULT IfaceCalling CDb_Connector::Connect(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password, db::IDb_Connection **connection) {
	try {
		return Manufacture_Object<CDb_Connection, db::IDb_Connection>(connection, host, provider, port, name, user_name, password);
	}
	catch (...) {
		return E_FAIL;
	}
}