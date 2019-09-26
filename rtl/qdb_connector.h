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
 * Univerzitni 8
 * 301 00, Pilsen
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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

#include "../iface/FilterIface.h"
#include "../rtl/DbLib.h"
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

HRESULT IfaceCalling Setup_Filter_DB_Access(glucose::IFilter *filter, const void* data);