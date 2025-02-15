#include "arenaAllocator.h"
#include <stdlib.h>

ArenaAllocator::ArenaAllocator(size_t size) : arena_size(size), offset(0) {
    memory = static_cast<char *>(std::malloc(arena_size));

    if (!memory) {
      throw std::bad_alloc();
    }
}

ArenaAllocator::~ArenaAllocator() {
  std::free(memory);
}

void ArenaAllocator::reset() {
    offset = 0;
}