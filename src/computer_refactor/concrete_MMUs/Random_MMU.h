#ifndef RANDOM_MMU_H
#define RANDOM_MMU_H

#include <cstdlib>
#include "../Page.h"
#include "../MMU.h"


class Random_MMU: public MMU {
public:
    Random_MMU(){}
    int paging(Page to_insert_page) {
        return rand()%MEMORY_SIZE;
    }
        
};

#endif
