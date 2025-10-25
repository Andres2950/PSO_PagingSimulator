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
                return HIT_COST;
            }

            state.memory->goToStart();
            replace_page(to_insert, state.memory->getSize()-1, state);
            
            return FAULT_COST;
        }
};

#endif //FIFO_H
