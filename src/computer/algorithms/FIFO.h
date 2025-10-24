#ifndef FIFO_H
#define FIFO_H

#include "../Page.h"
#include "Algorithm.h"
#include "../../data_structures/List.h"
#include "../../constants.h"

class FIFO: public Algorithm {
    public:
        FIFO(){}
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        int execute(Page to_insert, StatePerron& state) {
            if (state.memory->contains(to_insert)) {
                printf("HIT: %d\n", to_insert.id);
                state.currentTime += HIT_COST;
                return HIT_COST;
            }
            /*if (state.memory->getSize() < MEMORY_SIZE){
                int index = state.memory->getSize();
                to_insert.m_addr = index;
                to_insert.is_loaded = 1;
                to_insert.load_t = 0;
                state.memory->append(to_insert);
                state.currentTime += FAULT_COST;
                printf("Cache:");
                state.memory->print();
                printf("\n");
                return FAULT_COST;
            }*/

            state.memory->goToStart();
            replace_page(to_insert, state.memory->getSize()-1, state);
            
            printf("Cache:");
            state.memory->print();
            printf("\n");
            state.currentTime += FAULT_COST;

            return FAULT_COST;
        }
};

#endif //FIFO_H
