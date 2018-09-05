/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Technicka 8
 * 314 06, Pilsen
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) For non-profit, academic research, this software is available under the
 *    GPLv3 license. When publishing any related work, user of this software
 *    must:
 *    1) let us know about the publication,
 *    2) acknowledge this software and respective literature - see the
 *       https://diabetes.zcu.cz/about#publications,
 *    3) At least, the user of this software must cite the following paper:
 *       Parallel software architecture for the next generation of glucose
 *       monitoring, Proceedings of the 8th International Conference on Current
 *       and Future Trends of Information and Communication Technologies
 *       in Healthcare (ICTH 2018) November 5-8, 2018, Leuven, Belgium
 * b) For any other use, especially commercial use, you must contact us and
 *    obtain specific terms and conditions for the use of the software.
 */

#pragma once

#ifdef _WIN32
	#define IfaceCalling __stdcall
#else
	#define IfaceCalling
#endif

#include <memory>

#include "../rtl/hresult.h"
#include "../rtl/guid.h"
/*
  Note that we have to split this class into interface and its implementation,
  as we can use several different compilers. This is to guarantee that the method
  implementations will be consistent - i.e. that each object will have the methods
  compiled with the same compiler.

*/
namespace refcnt {

	class IUnique_Reference {
	public:
		virtual ULONG IfaceCalling Release() = 0;					//releases allocated memory using the right allocator, returns 0
	};

	class IReferenced {
		/* Actually, this is IUnknown of the WinAPI's COM
		   HRESULT and ULONG are used to allow possible and easy interoperability
		   accross different compilers and languages on Windows
		*/

	public:
		virtual HRESULT IfaceCalling QueryInterface(const GUID*  riid, void ** ppvObj) = 0;
		virtual ULONG IfaceCalling AddRef() = 0;
		virtual ULONG IfaceCalling Release() = 0;
	};

	template <typename S, typename I>
	//this one is designed for extending std::shared_ptr via inheritance
	S make_shared_reference_ext(I *obj, bool add_reference) {
		if ((add_reference) && (obj != nullptr)) obj->AddRef();
		S result;
		result.reset(obj, [](I* obj_to_release) {if (obj_to_release != nullptr) obj_to_release->Release(); });
		//shared_ptr will overtake the assignment operations and maintain its own counter
		//when shared_ptr's counter will come to zero, referenced's relase will take action
		return result;
	}

	template <typename I>
	std::shared_ptr<I>  make_shared_reference(I *obj, bool add_reference) {
		return make_shared_reference_ext<std::shared_ptr<I>, I>(obj, add_reference);

		/*
		if (add_reference) obj->AddRef();
		std::shared_ptr<I> result(obj, [](I* obj_to_release) {obj_to_release->Release(); });
			//shared_ptr will overtake the assignment operations and maintain its own counter
			//when shared_ptr's counter will come to zero, referenced's relase will take action
		return result;
		*/
	}

	template <typename I, typename Q>
	void Query_Interface(I *obj, const GUID &id, std::shared_ptr<Q> &target) {
		Q* queried;
		if (obj->QueryInterface(&id, reinterpret_cast<void**>(&queried)) == S_OK)
			target.reset(queried, [](Q* obj_to_release) { if (obj_to_release != nullptr) obj_to_release->Release(); });
	}

	template <typename T>
	bool Shared_Valid_All(const T& a) {
		return a.get() != nullptr;
	}

	template <typename T, typename... Args>
	bool Shared_Valid_All(const T& a, const Args&... args) {
		bool result = a.get() != nullptr;
		if (result) result &= Shared_Valid_All(args...);
		return result;
	}


	template <typename T>
	class IVector_Container : public virtual refcnt::IReferenced {
	public:
		virtual HRESULT set(T *begin, T *end) = 0;		//T may point to e.g., IReferenced, thus cannot be const
		virtual HRESULT add(T *begin, T *end) = 0;
		virtual HRESULT get(T **begin, T **end) const = 0;		
		virtual HRESULT empty() const = 0;
	};

	using str_container = IVector_Container<char>;
	using wstr_container = IVector_Container<wchar_t>;
	using wstr_list = IVector_Container<wstr_container*>;
	using double_container = IVector_Container<double>;	
}