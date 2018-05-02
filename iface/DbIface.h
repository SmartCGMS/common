#pragma once

#include "../rtl/guid.h"
#include "referencedIface.h"

namespace db {

	class IDb_Connection : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Get_Raw(void **qdb) = 0;
			//for now, it returns *QSqlDatabase
	};

	class IDb_Connector : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Connect(const wchar_t *host, const wchar_t *provider, uint16_t port, const wchar_t *name, const wchar_t *user_name, const wchar_t *password, IDb_Connection **connection) = 0;
	};

	constexpr GUID Db_Sink_Filter =  { 0x2bdd102e, 0xc401, 0x4ddc,{ 0x88, 0x16, 0xf, 0xb1, 0x51, 0x87, 0x9e, 0x48 } };
	class IDb_Sink : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Set_Connector(IDb_Connector *connector) = 0;
	};

}