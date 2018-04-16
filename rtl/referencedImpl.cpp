#include "referencedImpl.h"

#include "manufactory.h"
#include <limits>
#include <algorithm>

#undef max
#undef min

namespace refcnt {

	#ifdef __INTEL_COMPILER
	[[maybe_unused]] 
	#endif
	HRESULT CReferenced::QueryInterface(/*REFIID */ void*  riid, void ** ppvObj) {
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

	

	ULONG IfaceCalling CNotReferenced::AddRef() {
		return std::numeric_limits<ULONG>::max();
	}

	ULONG IfaceCalling CNotReferenced::Release() {
		return std::numeric_limits<ULONG>::max();
	}



	std::wstring WChar_Container_To_WString(refcnt::wstr_container *container) {
		return Container_To_Vector<wchar_t, std::wstring>(container);
	}

	wstr_container* WString_To_WChar_Container(const wchar_t* str) {
		return Create_Container<wchar_t>(str, str + wcslen(str));
	}

	bool WChar_Container_Equals_WString(wstr_container *container, const wchar_t* str, size_t offset, size_t maxCount) {
		wchar_t *cont_begin, *cont_end;

		if (container->get(&cont_begin, &cont_end) != S_OK) return false;	//cannot compare

		cont_begin += offset;
		if (cont_begin >= cont_end)
			return false;

		size_t cont_len = std::min(static_cast<size_t>(cont_end - cont_begin), maxCount);
		if (cont_len != wcslen(str)) return false;	//different size, thus not equal

		return wmemcmp(cont_begin, str, cont_len) == 0;
	}


}
