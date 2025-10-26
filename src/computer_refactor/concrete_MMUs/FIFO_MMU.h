#ifndef FIFO_MMU_H
#define FIFO_MMU_H

#include "../Page.h"
#include "../MMU.h"

class FIFO_MMU: public MMU {
public:
    FIFO_MMU(){
     for (int i = 0; i < MEMORY_SIZE; ++i)
       memory[i] = -1;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        int time_loaded = -1;
        for (int i = 0; i < MEMORY_SIZE; i++){
            Page page = disk[memory[i]];
            if (page.load_t > time_loaded) {
                time_loaded = page.load_t;
                to_remove_index = i;
            }
        }
        return to_remove_index;
    }
        
};

#endif
