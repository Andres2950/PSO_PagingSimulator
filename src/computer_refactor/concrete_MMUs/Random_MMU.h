#ifndef RANDOM_MMU_H
#define RANDOM_MMU_H

#include "../MMU.h"
#include <cstdlib>

class Random_MMU : public MMU {
public:
  Random_MMU() {
    for (int i = 0; i < MEMORY_SIZE; ++i)
      memory[i] = -1;
  }

  int paging() { return rand() % MEMORY_SIZE; }
};

#endif
