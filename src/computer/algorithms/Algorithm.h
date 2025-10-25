#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../Page.h"
#include "../../data_structures/List.h"
#include "../../constants.h"

typedef struct {
    List<Page>* memory;
    List<Page>* disk;
    int currentTime;
    int to_insert_i;
} StatePerron;

class Algorithm {
    public:
        Algorithm(){}
        virtual ~Algorithm() = default;
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        // Se pasa el estado por referencia para modificar lo que hay adentro\
        // Asi no hay que tocar nada luego de llamar al algoritmo
        virtual int execute(Page to_insert, StatePerron& state) =0;

        // Esta funcion supone que el pos interno de memory apunta a la pagina que se va a reemplazar
        void replace_page(Page to_insert, int insert_in, StatePerron& state) {
            Page inserting;
            int disk_index = state.disk->indexOf(to_insert);
            //Si esta en disco, se saca
            if (disk_index != -1){ 
                state.disk->goToPos(disk_index);
                inserting = state.disk->remove();
            } else {
                // Si la pagin a no esta en disco
                // Significa que es una nueva pagina, creada con new 
                // La creacion de la pagina se maneja fuera de la funcion
                // Simplemente se asigna al lugar
                inserting = to_insert;
            }

            // TODO: No tengo ni idea de cómo manejar l_addr
            // se saca la pagina de memoria y se asigna en disco
            Page removed = state.memory->remove();
            removed.is_loaded = 0;
            removed.m_addr = -1;
            removed.load_t = -1; // El tiempo que llevaba cargada la pagina se remueve
            removed.timestamp = -1;
            state.disk->append(removed);
            
            // se inserta donde se quiere
            inserting.is_loaded = 1;
            inserting.m_addr = state.memory->getPos();
            inserting.load_t = 0; //La nueva pagina lleva 0 segundos cargada
            removed.timestamp = state.currentTime;
            state.memory->goToPos(insert_in);
            state.memory->insert(inserting);
        }
};

#endif //ALGORITHM_H
