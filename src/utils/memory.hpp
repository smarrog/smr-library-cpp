#pragma once

#include <cstddef>
#include <utility>

namespace smr {
	class Memory {
	public:
		Memory() = delete;

		template <typename T>
		static T* allocate(size_t n) {
			return static_cast<T*>(operator new(n * sizeof(T)));
		}

		template <typename T>
		static void deallocate(T* buf) {
			operator delete(buf);
		}

		template <typename T>
		static void construct(T* buf) {
			new (buf) T();
		}

		template <typename T>
		static void construct(T* buf, T& other) {
			new (buf) T(other);
		}

		template <typename T>
		static void construct(T* buf, T&& other) {
			new (buf) T(std::forward(other));
		}

		template <typename T>
		static void destroy(T* buf) {
			buf->~T();
		}
	};
}