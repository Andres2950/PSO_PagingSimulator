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
                state.memory->goToPos(mem_index);
                Page& page = state.memory->getElementRef();
                page.mark = 1;
                return HIT_COST;
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
             
            return FAULT_COST;
        };
};

#endif // SECOND_CHANCE_H
