#ifndef SECOND_CHANCE_MMU_H
#define SECOND_CHANCE_MMU_H

#include "../Page.h"
#include "../MMU.h"
#include <vector>


class Second_Chance_MMU: public MMU {
public:
    Second_Chance_MMU(){
        for (int i = 0; i < MEMORY_SIZE; ++i)
           memory[i] = -1;
    }
    void mark_used_inRAM(Page page){
        page.mark = 1;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        std::vector<int> vec;
        // se copia la memoria en un vector
        for (int i = 0; i < MEMORY_SIZE; ++i)
            vec.push_back(memory[i]);
        while(true){
            // si el tamaño es 0, todas las marcas se expiraron
            // Se vuelve a copiar para encontrar el mas viejo
            if (vec.size() == 0) {
                for (int i = 0; i < MEMORY_SIZE; ++i)
                    vec.push_back(memory[i]);
            }

            int time_loaded = -1;
            Page *page;
            // se encuentra la mas vieja de los del vector
            for (int i = 0; i < vec.size(); ++i){
                page = &(disk[vec[i]]);
                if (page->load_t > time_loaded){
                    time_loaded = page->load_t;
                    to_remove_index = i;
                }
            }
            page = &(disk[vec[to_remove_index]]);
            // si hay marca se saca del vector y se le da una nueva oportunidad
            if (page->mark){
                vec.erase(vec.begin() + to_remove_index);
                to_remove_index = -1;
                page->mark = 0; // se expira
                continue;
            }

            // si no hay marca encontramos el indice de memory y lo devolvemos
            int victim = vec[to_remove_index];
            for (int i = 0; i < MEMORY_SIZE; ++i){
                if (memory[i] == victim)
                    return i;
            }
        
        }
    }
        
};

#endif
