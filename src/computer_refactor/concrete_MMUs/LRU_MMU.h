#ifndef LRU_MMU_H
#define LRU_MMU_H

#include <limits.h>
#include "../Page.h"
#include "../MMU.h"

class LRU_MMU: public MMU {
public:
    LRU_MMU(){
        for (int i = 0; i < MEMORY_SIZE; ++i)
            memory[i] = -1;
    }
    void mark_used(Page page) {
        page.mark = time;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        int least_recent = INT_MAX;
        for (int i = 0; i < MEMORY_SIZE; ++i){
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
