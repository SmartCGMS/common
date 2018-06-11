#include "referencedImpl.h"

#include "manufactory.h"
#include <limits>
#include <algorithm>

#undef max
#undef min

namespace refcnt {

	HRESULT CReferenced::QueryInterface(const GUID*  riid, void ** ppvObj) {
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

	std::string Char_Container_To_String(str_container *container) {
		return Container_To_Vector<char, std::string>(container);
	}


	std::wstring WChar_Container_To_WString(refcnt::wstr_container *container) {
		return Container_To_Vector<wchar_t, std::wstring>(container);
	}

	std::shared_ptr<wstr_container> WString_To_WChar_Container_shared(const wchar_t* str) {
		const size_t len = str != nullptr ? wcslen(str) : 0;
		return Create_Container_shared<wchar_t>(str, str + len);
	}

	wstr_container* WString_To_WChar_Container(const wchar_t* str) {
		const size_t len = str != nullptr ? wcslen(str) : 0;
		return Create_Container<wchar_t>(str, str + len);
	}

	bool WChar_Container_Equals_WString(wstr_container *container, const wchar_t* str, size_t offset, size_t maxCount) {
		wchar_t *cont_begin, *cont_end;

		if (container->get(&cont_begin, &cont_end) != S_OK || !str) return false;	//cannot compare

		cont_begin += offset;
		if (cont_begin >= cont_end)
			return false;

		const size_t cont_len = static_cast<size_t>(cont_end - cont_begin);
		if (maxCount == (size_t)-1 && cont_len != wcslen(str)) return false;	//different size, thus not equal

		return wmemcmp(cont_begin, str, std::min(cont_len, maxCount > 0 ? maxCount : wcslen(str))) == 0;
	}


	void Swstr_container::set(const wchar_t *str) {
		if (operator bool()) {
			if (str != nullptr) get()->set(str, str + wcslen(str));
			else get()->set(nullptr, nullptr);
		}		
	}


	bool Swstr_container::operator==(const wchar_t *other) const {
		if (!operator bool()) return false;

		return WChar_Container_Equals_WString(get(), other);
	}

}

