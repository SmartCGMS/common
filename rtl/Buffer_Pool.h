#pragma once

#include <functional>
#include <tbb/concurrent_queue.h>

namespace internal {
	template <typename T>
	using TResize_Lambda = std::function<void(T& container, const size_t minimum_size)>;
}

template <typename T>
class CBuffer_Pool {
protected:
	tbb::concurrent_queue<T> mPool;
	internal::TResize_Lambda<T> mResize_Func;
public:
	CBuffer_Pool(internal::TResize_Lambda<T> resize_func) : mResize_Func(resize_func) {};

	T pop(const size_t minimum_size) {
		T result;
		mPool.try_pop(result);
		mResize_Func(result, minimum_size);
		return result;
	}


	void push(T&& elem) {
		mPool.push(elem);
	}
};


template <typename T>
class CPooled_Buffer {
		//cannot inherite due to Eigen => T element
protected:
	CBuffer_Pool<T> &mPool;
public:
	CPooled_Buffer(CBuffer_Pool<T> pool, const size_t minimum_size) : mPool(pool) { 
		element = mPool.pop(minimum_size);
	}

	~CPooled_Buffer() {
		mPool.push(std::move(element));
	}

	T element;
};