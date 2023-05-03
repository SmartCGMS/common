/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#include "qdb_connector.h"

#ifndef DDO_NOT_USE_QT

#include "../rtl/manufactory.h"
#include "../utils/DebugHelper.h"
#include "../utils/QtUtils.h"
#include <QtCore/QUuid>
#include <QtCore/QVariant>
#include <QtSql/QSqlError>

#undef max


CDb_Connector db_connector{};



CDb_Query::CDb_Query(QSqlDatabase &db, const wchar_t *statement) : mQuery(QSqlQuery{ db }) {
	if (!mQuery.prepare(QString::fromWCharArray(statement))) {
		const auto msg = mQuery.lastError().databaseText().toStdString();
		const auto msg2 = mQuery.lastError().driverText().toStdString();
		dprintf("Query error: %s\r\nAssociated driver error: %s\r\n", msg.c_str(), msg2.c_str());
		throw std::invalid_argument{ "Malformed SQL statement." };
	}
}


HRESULT IfaceCalling CDb_Query::Bind_Parameters(const db::TParameter *values, const size_t count) {

	for (size_t i = 0; i < count; i++) {

		switch (values[i].type) {
			default:
			case db::NParameter_Type::ptNull:		mQuery.addBindValue(QVariant{});
													break;

			case db::NParameter_Type::ptInt64:		mQuery.addBindValue(static_cast<qlonglong>(values[i].integer));
													break;

			case db::NParameter_Type::ptDouble:		mQuery.addBindValue(values[i].dbl);
													break;

			case db::NParameter_Type::ptWChar:		mQuery.addBindValue( QString::fromWCharArray(values[i].str));
													break;

			case db::NParameter_Type::ptBool:		mQuery.addBindValue(values[i].boolean != FALSE ? true : false);
													break;

			case db::NParameter_Type::ptGuid:		mQuery.addBindValue(GUID_To_QUuid(values[i].id));
													break;

			case db::NParameter_Type::ptBinaryObect:mQuery.addBindValue(QByteArray(reinterpret_cast<const char*>(values[i].binary_object.data), static_cast<int>(values[i].binary_object.size)));
													break;
		}
	}

	return S_OK;
}

HRESULT IfaceCalling CDb_Query::Get_Next(db::TParameter* const values, const size_t count) {
	if (!mExecuted) mExecuted = mQuery.exec();
	if (!mExecuted) {
                auto msg =mQuery.lastError().driverText().toStdString();
                dprintf(msg.c_str());
		dprintf("\n");
                msg = mQuery.lastError().databaseText().toStdString();
                dprintf(msg.c_str());
		dprintf("\n");
		return E_FAIL;
	}

	if (mQuery.next()) {
		mResult_String_Row.resize(std::max(mResult_String_Row.size(), count));

		for (size_t i = 0; i < count; i++) {
			QVariant db_value = mQuery.value(static_cast<int>(i));
			//first, we had to attempt to read the value as some db drivers
			//will provide valid IsNull() only after that
			if (!mQuery.isNull(static_cast<int>(i))) {

				bool ok_test;
				QString tmp_str;
				QByteArray tmp_byte_arr;

				switch (values[i].type) {
					case db::NParameter_Type::ptInt64:		values[i].integer = static_cast<int64_t>(db_value.toLongLong(&ok_test));
															if (!ok_test)
																values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptDouble:		values[i].dbl = db_value.toDouble(&ok_test);
															if (!ok_test)
																values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptWChar:		tmp_str = db_value.toString();
															if (tmp_str.size() > 0) {
																mResult_String_Row[i] = tmp_str.toStdWString();
																values[i].str = const_cast<wchar_t*>(mResult_String_Row[i].c_str());
															}
															else
																values[i].type = db::NParameter_Type::ptNull;
															break;

					case db::NParameter_Type::ptBool:		values[i].boolean = db_value.toBool() ? TRUE : FALSE;
															break;

					case db::NParameter_Type::ptGuid:		values[i].id = QUuid_To_GUID(db_value.toUuid());
															break;

					case db::NParameter_Type::ptBinaryObect:	// this might be potentially dangerous as we are assigning temporary object
																// but the result will be valid as long as the query is in memory
																// TODO: solve this better (COM-compatible binary object representation)
															tmp_byte_arr = db_value.toByteArray();
															values[i].binary_object.data = reinterpret_cast<uint8_t*>(tmp_byte_arr.data());
															values[i].binary_object.size = tmp_byte_arr.size();
															break;

					default: break;
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

CDb_Connection::CDb_Connection(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password) : 
	mConnection_Name("QDB_Connection_" + QUuid::createUuid().toString()) {
	
	mDb = QSqlDatabase::addDatabase(QString::fromWCharArray(provider), mConnection_Name);
	
	mDb.setHostName(QString::fromWCharArray(host));
	if (port != 0) mDb.setPort(port);
	mDb.setDatabaseName(QString::fromWCharArray(name));
	mDb.setUserName(QString::fromWCharArray(user_name));
	mDb.setPassword(QString::fromWCharArray(password));

	if (!mDb.open()) {
                auto msg = mDb.lastError().driverText().toStdString();
                dprintf(msg.c_str());
		dprintf("\n");
                msg = mDb.lastError().databaseText().toStdString();
                dprintf(msg.c_str());
		dprintf("\n");
		throw "Cannot open database";
	}
}

CDb_Connection::~CDb_Connection() {
	mDb.close();
	//QSqlDatabase::removeDatabase(mConnection_Name);
}


HRESULT IfaceCalling CDb_Connection::Query(const wchar_t *statement, db::IDb_Query **query) {
	try {
		return Manufacture_Object<CDb_Query, db::IDb_Query>(query, QSqlDatabase::database(mConnection_Name), statement);
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


HRESULT IfaceCalling Setup_Filter_DB_Access(scgms::IFilter *filter, const void* data) {
	HRESULT rc = S_OK;
#ifndef SMARTCGMS_NO_DB
	{
		db::SDb_Sink db_sink;
		refcnt::Query_Interface<scgms::IFilter, db::IDb_Sink>(filter, db::Db_Sink_Filter, db_sink);
		if (db_sink) rc = db_sink->Set_Connector(static_cast<db::IDb_Connector*>(&db_connector));
	}
#endif

	return rc;
}

#endif
