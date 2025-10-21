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

            /*
            //Si se encuentra la pagina en disco, se quita
            Page insert;
            int disk_index = state.disk->indexOf(to_insert);
            if (disk_index != -1){
                state.disk->goToPos(disk_index);
                insert = state.disk->remove();
            } else {
                // Si la pagin a no esta en disco
                // Significa que es una nueva pagina, creada con new 
                // Esto se maneja fuera de la funcion
                insert = to_insert;
            }

            Page removed = state.memory->remove();
            state.disk->append(removed);
            state.memory->append(insert);
            state.currentTime += FAULT_COST;*/
            replace_page(to_insert, state);

            printf("Cache:");
            state.memory->print();
            printf("\n");
            
        }
};

#endif //FIFO_H
