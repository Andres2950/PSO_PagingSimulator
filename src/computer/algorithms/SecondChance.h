#ifndef SECOND_CHANCE_H
#define SECOND_CHANCE_H

#include <stdlib.h>
#include "../../constants.h"
#include "../Page.h"
#include "../../data_structures/List.h"
#include "Algorithm.h"

class SecondChance: public Algorithm {
    public:
        SecondChance(){}
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        int execute(Page to_insert, StatePerron& state){
            int mem_index = state.memory->indexOf(to_insert);
            if (mem_index != -1) {
                printf("HIT: %d\n", to_insert.id);
                state.currentTime += HIT_COST;
                state.memory->goToPos(mem_index);
                Page& page = state.memory->getElementRef();
                page.mark = 1;
                //to_insert.mark = 1;
                return HIT_COST;
            }
            if (state.memory->getSize() < MEMORY_SIZE){
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
            }
            
            state.memory->goToStart(); //El primero que se inserto
            while (true){
                if (state.memory->atEnd()) state.memory->goToStart();
                Page& page = state.memory->getElementRef();
                // can be removed
                if(page.mark == 0) {
                    break; 
                } else {
                    // expire the mark
                    page.mark = 0;
                    state.memory->next();
                }
            }

            replace_page(to_insert, state.memory->getSize()-1, state);
             
            state.currentTime += FAULT_COST;
            printf("Cache:");
            state.memory->print();
            printf("\n");
            return FAULT_COST;
        };
};

#endif // SECOND_CHANCE_H
