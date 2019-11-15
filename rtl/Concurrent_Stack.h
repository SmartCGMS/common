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

#include <atomic>

namespace solver {
	namespace internal {

		template <typename T>
		struct TLink {
			TLink* next;
			T data;
		};

		class CScoped_SpinLock {
		protected:
			::std::atomic_flag &mSpin_Flag;
		public:
			CScoped_SpinLock(::std::atomic_flag &flag) : mSpin_Flag(flag) {
				while (flag.test_and_set(::std::memory_order_acquire)) {}
			}

			~CScoped_SpinLock() { mSpin_Flag.clear(::std::memory_order_release); }
		};
	}


	template <typename T>
	class CConcurrent_Stack {
	protected:
		::std::atomic_flag mSpin_Flag;
		internal::TLink<T>* mAvailable_List = nullptr;
	public:
		~CConcurrent_Stack() {
			//we need to walk the available list and free it

			while (mAvailable_List) {
				internal::TLink<T> *next_item = mAvailable_List->next;
				delete mAvailable_List;
				mAvailable_List = next_item;
			}
		}

		void push(T& element) {
			internal::TLink<T> *new_item = new internal::TLink<T>;
			new_item->data = std::move(element);

			{
				internal::CScoped_SpinLock lock{ mSpin_Flag };
				new_item->next = mAvailable_List;
				mAvailable_List = new_item;
			}
		}

		bool try_pop(T &dst) {
			/*
				The push operation could have been constructed using the spinlock itself.
				However, concurrent pop would require an extra state, which would complicate the code.
				Do not forget, that for a spinlock-pop, we would have to atomically load not only the list-head,
				but the next item as well. When reading the next pointer, a different core can possibly release the memory,
				where the next is stored. Hence, the spinlock-pop operation would not lead to a more effective code.
				(unless we would use a platform specific instructions, for which would have to implement a fallback path anyway)
				Hence, a global spinlock is a nicer solution.
			*/

			internal::TLink<T> *removed_item = nullptr;
			{
				internal::CScoped_SpinLock lock{ mSpin_Flag };
				removed_item = mAvailable_List;
				if (removed_item) mAvailable_List = removed_item->next;
				else return false;
			}

			dst = std::move(removed_item->data);
			delete removed_item;

			return true;
		}
	};
}