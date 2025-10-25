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
                state.memory->goToPos(index);
                Page page = state.memory->getElementRef();
                page.mark = state.currentTime;
                return HIT_COST;
            }
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
          
            to_insert.mark = state.currentTime;
            state.memory->goToPos(pos);
            replace_page(to_insert, state.memory->getPos(), state);
            
            return FAULT_COST;
        }
}; 

#endif // LRU_H
