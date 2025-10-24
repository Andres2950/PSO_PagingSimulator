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
#include "../../constants.h"

enum ALGORITHM {
    FI_FO,
    SECOND_CHANCE,
    MRU,
    LRU,
    RANDOM
}

class MMU {
    public: 
        int ptr_count = 0;
        StatePerron state;
        //List<Page>* memory;
        //List<Page>* disk;
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
            int page_ammount = (size%PAGE_SIZE == 0)? size/PAGE_SIZE : size/PAGE_SIZE+1;
            List<Page>* list = new ArrayList<Page>();
            for (int i = 0; i < page_ammount; i++) {
                Page* page = new Page();
                list.append(page);
                algorithm.execute(page, state);
            }
            return page;
        }
        

        void use(int ptr){
          // algo hace
        }

        MMU(ALGORITHM algorithm){
            //Memoria real de 100 paginas
           switch (algorithm){
              case FI_FO: 
                break;
              case SECOND_CHANCE:
                break;
              case MRU:
                break;
              case LRU:
                break;
              case RANDOM:
                break;
           }
           state.currentTime time = 0;
           state.memory = new ArrayList<Page>(100); 
           state.disk = new ArrayList<Page>();
           ptr_table = new HashTable<int, ArrayList<Page>*>;
        }
        MMU(List<Page> future) {
            algorithm = new Optimal(future);
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
