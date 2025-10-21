#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../Page.h"
#include "../../data_structures/List.h"

typedef struct {
    List<Page>* memory;
    List<Page>* disk;
    int currentTime;
} StatePerron;

class Algorithm {
    public:
        Algorithm(){}
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        // Se pasa el estado por referencia para modificar lo que hay adentro\
        // Asi no hay que tocar nada luego de llamar al algoritmo
        virtual void execute(Page to_insert, StatePerron& state) =0;
};

#endif //ALGORITHM_H
