#ifndef MMU_H
#define MMU_H

#include "Page.h"
#include "../data_structures/List.h"
#include "../data_structures/ArrayList.h"
#include "../data_structures/Dictionary.h"
#include "../data_structures/HashTable.h"
#include "algorithms/Algorithm.h"
#include "algorithms/FIFO.h"
#include "algorithms/Optimal.h"
#include "algorithms/SecondChance.h"

class MMU {
    public: 
        static inline int ptr_count = 0;
        List<Page>* memory;
        List<Page>* disk;
        Algorithm *algorithm;
        //no lista de enteros algo diferente
        //Table debe contener todos los punteros
        // es una Dictionary <ptr_id, ptr_pages>
        // Estoy usando un arraylist pq List necesita ser un puntero 
        // El problema de eso es que HashTable no sabe si K y V son punteros entonces no los libera
        // Entonces x eso uso el tipo concreto
        Dictionary<int, ArrayList<Page>*>* ptr_table;

        int _new(int pid, int size){
          // retorna el ptr
          return 0;
        }

        void use(int ptr){
          // algo hace, guardar para el optimo?
        }

        MMU(int algorithm){
            //Memoria real de 100 paginas
           switch (algorithm){
              case 0:
                break;
              case 1:
                break;
              case 2:
                break;
              case 3:
                break;
              case 4:
                break;
              case 5:
                List<Page> *list = new ArrayList<Page>();
                this->algorithm = new Optimal(list);
           }
           memory = new ArrayList<Page>(100); 
           disk = new ArrayList<Page>();
           ptr_table = new HashTable<int, ArrayList<Page>*>;
        }
        ~MMU() {
            delete memory;
            delete disk;
            delete ptr_table;
        }
};


#endif
