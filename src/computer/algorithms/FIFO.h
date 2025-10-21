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
        void execute(Page to_insert, StatePerron& state) {
            if (state.memory->contains(to_insert)) {
                printf("HIT: %d\n", to_insert.id);
                state.currentTime += HIT_COST;
                return;
            }
            
            state.memory->goToStart(); //El primero que se inserto
            Page removed = state.memory->remove();
            state.disk->append(removed);
            state.memory->append(to_insert);
            state.currentTime += FAULT_COST;
            
            printf("Cache:");
            state.memory->print();
            printf("\n");
            
        }
};

#endif //FIFO_H
