#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include "../Page.h"
#include "Algorithm.h"
#include "../../data_structures/List.h"
#include "../../constants.h"

class Random: public Algorithm {
    public:
        Random(){}
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        int execute(Page to_insert, StatePerron& state) {
            if (state.memory->contains(to_insert)) {
                return HIT_COST;
            }

            state.memory->goToPos(rand()%state.memory->getSize());  
            replace_page(to_insert, state.memory->getPos(), state);
            
            return FAULT_COST;
        }
};

#endif //RANDOM_H
