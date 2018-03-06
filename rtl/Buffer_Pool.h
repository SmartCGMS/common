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