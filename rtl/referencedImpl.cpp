#include "referencedImpl.h"

#include "cfixes.h"

#include <limits>

namespace refcnt {


	HRESULT CReferenced::QueryInterface(/*REFIID */ void*  riid, void ** ppvObj) {
		unused(ppvObj);
		unused(riid);
		return E_NOTIMPL;
	}

	ULONG IfaceCalling CReferenced::AddRef() {
		return mCounter.fetch_add(1) + 1;
	}

	ULONG IfaceCalling CReferenced::Release() {
		ULONG rc = mCounter.fetch_sub(1) - 1;	//fetch_sub returns the old value!	 
		if (rc == 0) delete this;

		return rc;
	}


#undef max

	ULONG IfaceCalling CNotReferenced::AddRef() {
		return std::numeric_limits<ULONG>::max();
	}

	ULONG IfaceCalling CNotReferenced::Release() {
		return std::numeric_limits<ULONG>::max();
	}

}