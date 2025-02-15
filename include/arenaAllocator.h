#pragma once

#include <cstddef>
#include <new>

class ArenaAllocator {
private:
  size_t arena_size;
  size_t offset;
  char* memory;

public:

  explicit ArenaAllocator(size_t arena_size);
  ~ArenaAllocator();

  template<typename T>
  T* allocate();

  void reset();
};

template<typename T>
T* ArenaAllocator::allocate() {
    const size_t size = sizeof(T);

    if (offset + size > arena_size) throw std::bad_alloc();

    T* ptr = reinterpret_cast<T*>(memory + offset);
    offset += size;

    return new(ptr) T();
}

