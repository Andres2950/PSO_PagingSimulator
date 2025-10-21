#ifndef MMU_H
#define MMU_H

#include "Page.h"
#include "../data_structures/List.h"
#include "../data_structures/ArrayList.h"
#include "../data_structures/Dictionary.h"
#include "../data_structures/HashTable.h"

class MMU {
    public: 
        static inline int ptr_count = 0;
        List<Page>* memory;
        List<Page>* disk;
        //no lista de enteros algo diferente
        //Table debe contener todos los punteros
        // es una Dictionary <ptr_id, ptr_pages>
        // Estoy usando un arraylist pq List necesita ser un puntero 
        // El problema de eso es que HashTable no sabe si K y V son punteros entonces no los libera
        // Entonces x eso uso el tipo concreto
        Dictionary<int, ArrayList<Page>>* ptr_table;

        MMU(){
            //Memoria real de 100 paginas
           memory = new ArrayList<Page>(100); 
           // disco es 'infinito'
           // Por defecto el array se crea de 1024
           // Pero tiene la funcion expand incluida y se usa automaticamente
           // Entonces sirve perfectamente como infinito
           disk = new ArrayList<Page>();
           ptr_table = new HashTable<int, ArrayList<Page>>;
        }
        ~MMU() {
            delete memory;
            delete disc;
            delete ptr_table;
        }


};


#endif
