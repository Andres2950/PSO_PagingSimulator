#ifndef SECOND_CHANCE_H
#define SECOND_CHANCE_H

#include <stdlib.h>
#include "../../constants.h"
#include "../Page.h"
#include "../../data_structures/List.h"
#include "Algorithm.h"

class SecondChance: Algorithm {
    public:
        SecondChance(){}
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        void execute(Page to_insert, StatePerron& state){
            int mem_index = state.memory->indexOf(to_insert);
            if (mem_index != -1) {
                printf("HIT: %d\n", to_insert.id);
                state.currentTime += HIT_COST;
                state.memory->goToPos(mem_index);
                Page& page = state.memory->getElementRef();
                page.mark = 1;
                //to_insert.mark = 1;
                return;
            }
            
            state.memory->goToStart(); //El primero que se inserto
            while (true){
                Page& page = state.memory->getElementRef();
                // can be removed
                if(page.mark == 0) {
                    break; 
                } else {
                    // expire the mark
                    page.mark = 0;
                    if (state.memory->atEnd())
                        state.memory->goToStart();
                    else 
                        state.memory->next();
                }
            }
            /*
            //Si se encuentra la pagina en disco, se quita
            Page insert;
            int disk_index = state.disk->indexOf(to_insert);
            if (disk_index != -1){
                state.disk->goToPos(disk_index);
                insert = state.disk->remove();
                insert.d_addr = -1;
            } else {
                // Si la pagin a no esta en disco
                // Significa que es una nueva pagina, creada con new 
                // Esto se maneja fuera de la funcion
                // Simplemente se asigna al lugar
                insert = to_insert;
            }
            
            // Para asignarle la direccion fisica en memoria a la pagina
            // No estoy seguro de como manejar la direccion logica
            state.memory->goToEnd();
            insert.m_addr = state.memory->getPos();
            insert.is_loaded = true;

            Page removed = state.memory->remove();
            state.disk->append(removed);
            state.memory->append(insert);
            state.currentTime += FAULT_COST;
            */

            replace_page(to_insert, state);
             
            printf("Cache:");
            state.memory->print();
            printf("\n");
            
        };
};

#endif // SECOND_CHANCE_H
