#pragma once

#include <cstddef>
#include <utility>

template <typename T>
class RawMemory {
private:
    static T* allocate(size_t n) {
        return static_cast<T*>(operator new (n * sizeof(T)));
    }

    static void deallocate(T* buf) {
        operator delete(buf);
    }
public:
    RawMemory() = default;
    explicit RawMemory(size_t n) : _buf(allocate(n)), _capacity(n) { }
    RawMemory(const RawMemory&) = delete;
    RawMemory(RawMemory&& other) noexcept { swap(other); }

    ~RawMemory() { deallocate(_buf); }

    RawMemory& operator = (const RawMemory&) = delete;
    RawMemory& operator = (RawMemory&& other) noexcept { swap(other); return *this; }

    void swap(RawMemory& other) {
        std::swap(_buf, other._buf);
        std::swap(_capacity, other._capacity);
    }

    size_t getCapacity() const { return _capacity; }

    T* operator + (size_t i) { return _buf + i; }
    const T* operator + (size_t i) const { return _buf + i; }
    T& operator [] (size_t i) { return _buf[i]; }
    const T& operator [] (size_t i) const { return _buf[i]; }
    T* operator * () { return _buf; }
    const T* operator * () const { return _buf; }
private:
    T* _buf = nullptr;
    size_t _capacity = 0;
};