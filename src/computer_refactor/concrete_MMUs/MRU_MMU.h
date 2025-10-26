#ifndef MRU_MMU_H
#define MRU_MMU_H

#include "../Page.h"
#include "../MMU.h"

class MRU_MMU: public MMU {
public:
    MRU_MMU(){
        for (int i = 0; i < MEMORY_SIZE; ++i)
           memory[i] = -1;
    }
    void mark_used(Page page) {
        page.mark = time;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        int most_recent = -1;
        for (int i = 0; i < MEMORY_SIZE; ++i){
            int timestamp = disk[memory[i]].mark;
            if (timestamp > most_recent) {
                most_recent = timestamp;
                to_remove_index = i;
            }
        }
        return to_remove_index;
    }
        
};

#endif
