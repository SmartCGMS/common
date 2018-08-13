#pragma once

#include "../rtl/guid.h"
#include "referencedIface.h"

namespace db {

	enum class NParameter_Type : size_t {		
		ptNull,
		ptInt64,
		ptDouble,
		ptWChar,
		ptBool
	};

	struct TParameter {
		NParameter_Type type;
		union {			
			int64_t integer;
			double dbl;
			wchar_t *str;
			bool boolean;
		};
	};

	static constexpr int64_t Anonymous_Subject_Identifier = -2;
	static constexpr int64_t New_Subject_Identifier = -1;

	class IDb_Query : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Bind_Parameters(const TParameter *values, const size_t count) = 0;
			//binds count arguments, for which the following rules aplies

		virtual HRESULT IfaceCalling Get_Next(TParameter* const values, const size_t count) = 0;
			//reads a result row, particularly reads values at given indexes of given types and sets pointers/values 
			//null values are indicated with the type member
			//pointers are valid only until the next call of Get_Next/Cancel or dctor
			//returns S_OK if a row of a result has been succesfully fetched (internally calls exec on before fetching the very first row)

			
		virtual HRESULT IfaceCalling Cancel() = 0;
			//cancels the current query so it can be restarted with the Get_Next sequence			
	};

	class IDb_Connection : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT IfaceCalling Query(const wchar_t *statement, IDb_Query **query) = 0;
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