#ifndef LRU_MMU_H
#define LRU_MMU_H

#include "../MMU.h"
#include <limits.h>

class LRU_MMU : public MMU {
public:
  LRU_MMU() {
    for (int i = 0; i < MEMORY_SIZE; ++i)
      memory[i] = -1;
  }

  void mark_used(int pageId) { disk[pageId].mark = time; }

  int paging() {
    int to_remove_index = -1;
    int least_recent = INT_MAX;
    for (int i = 0; i < MEMORY_SIZE; ++i) {
      int timestamp = disk[memory[i]].mark;
      if (timestamp < least_recent) {
        least_recent = timestamp;
        to_remove_index = i;
      }
    }
    return to_remove_index;
  }
};

#endif
