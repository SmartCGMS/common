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

#include <functional>
#include <tbb/concurrent_queue.h>

namespace internal {
	template <typename T>
	using TResize_Lambda = std::function<void(T& container, const size_t minimum_size)>;
}

template <typename T>
class CBuffer_Pool;

template <typename T>
class CPooled_Buffer {
	//cannot inherite due to Eigen => T element
protected:
	CBuffer_Pool<T> &mPool;
	T mElement;
public:
	CPooled_Buffer(CBuffer_Pool<T> &pool, T& element) : mPool(pool), mElement(std::move(element)) {
	}

	~CPooled_Buffer() {
		mPool.push(std::move(mElement));
	}

	T& element() {
		return mElement;
	};
};

template <typename T>
class CBuffer_Pool {
	friend CPooled_Buffer<T>;
protected:
	tbb::concurrent_queue<T> mPool;
	internal::TResize_Lambda<T> mResize_Func;

	void push(T&& elem) {
		mPool.push(elem);
	}

public:
	CBuffer_Pool(internal::TResize_Lambda<T> resize_func) : mResize_Func(resize_func) {};

	CPooled_Buffer<T> pop(const size_t minimum_size) {
		T buffer;
		mPool.try_pop(buffer);
		mResize_Func(buffer, minimum_size);
		
		return CPooled_Buffer<T> (*this, buffer);
	}

};