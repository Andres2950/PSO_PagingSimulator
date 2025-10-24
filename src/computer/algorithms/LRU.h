#ifndef LRU_H
#define LRU_H

#include <limits.h>
#include "../Page.h"
#include "Algorithm.h"
#include "../../data_structures/List.h"
#include "../../constants.h"

class LRU: public Algorithm {
        int execute(Page to_insert, StatePerron& state) {
            int index = state.memory->indexOf(to_insert);
            if (index != -1) {
                printf("HIT: %d\n", to_insert.id);
                state.memory->goToPos(index);
                Page page = state.memory->getElementRef();
                page.mark = state.currentTime;
                state.currentTime += HIT_COST;
                return HIT_COST;
            }
            /*if (state.memory->getSize() < MEMORY_SIZE){
                int index = state.memory->getSize();
                to_insert.m_addr = index;
                to_insert.is_loaded = 1;
                to_insert.load_t = 0;
                to_insert.mark = 0;
                state.memory->append(to_insert);
                state.currentTime += FAULT_COST;
                printf("Cache:");
                state.memory->print();
                printf("\n");
                return FAULT_COST;
            }*/

            // To find the most recently used
            // The one that has the biggest timestamp
            int least_recent = INT_MAX;
            int pos = -1;
            for (state.memory->goToStart(); !state.memory->atEnd(); state.memory->next()) {
                int timestamp = state.memory->getElement().mark;
                if (timestamp < least_recent) {
                    least_recent = timestamp;
                    pos = state.memory->getPos();
                }
            }
           
            state.memory->goToPos(pos);
            replace_page(to_insert, state.memory->getPos(), state);
            
            printf("Cache:");
            state.memory->print();
            printf("\n");
            state.currentTime += FAULT_COST;

            return FAULT_COST;
        }
}; 

#endif // LRU_H
