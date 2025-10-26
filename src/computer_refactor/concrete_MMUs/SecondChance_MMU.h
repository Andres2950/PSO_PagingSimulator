#ifndef SECOND_CHANCE_MMU_H
#define SECOND_CHANCE_MMU_H

#include "../Page.h"
#include "../MMU.h"


class Second_Chance_MMU: public MMU {
public:
    Second_Chance_MMU(){}
    void mark_used_inRAM(Page page){
        page.mark = 1;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        while(to_remove_index == -1){
            int time_loaded = -1;
            // se encuentra la mas vieja
            for (int i = 0; i < MEMORY_SIZE; ++i){
                Page *page = &(disk[memory[i]]);
                if (page->load_t > time_loaded){
                    time_loaded = page->load_t;
                    to_remove_index = i;
                }
            }
        }
        return to_remove_index;
    }
        
};

#endif
