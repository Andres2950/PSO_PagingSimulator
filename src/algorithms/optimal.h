#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "../data_structures/List.h"


int optimal(int k, List<int> &peticiones);


#define OPTIMAL_IMPLMENTATION
#include <stdio.h>
int optimal(int k, List<int>& peticiones) {
    List<int>* cache = new ArrayList<int>();
    //Inserta y remueve las cosas hasta que se llene el cache
    for (int i = 0; i < k; ++i) {
        peticiones.goToStart();
        cache->goToStart();
        cache->append(peticiones.remove());
    }
    int fallos = k;
    printf("Starting cache: ");
    cache->print();
    printf("\n");

    // for pe_i, pe in enumerate(peticiones)
    for (int petition_i = 0; petition_i < peticiones.getSize(); ++petition_i) {
        peticiones.goToPos(petition_i);
        // if pe in cache
        if (cache->contains(peticiones.getElement())) {
            printf("HIT: %d\n", peticiones.getElement());
            continue;
        }

        int to_remove_pag = 0;
        int to_remove = -1;
        //for pag_i, pag in enumerate(cache)
        for (int pag_i = 0; pag_i < cache->getSize(); ++pag_i) {
            List<int>* tmp = new ArrayList<int>();
            // tmp = peticiones[pe_i:]
            for (int i = petition_i; i < peticiones.getSize(); ++i) {
                peticiones.goToPos(i);
                tmp->append(peticiones.getElement()); 
            }
            // if pag not int peticiones[pe_i:]
            cache->goToPos(pag_i);
            if (!tmp->contains(cache->getElement())){
                to_remove_pag = pag_i;
                break;
            }
            delete tmp;
            // for j in range(pe_i+1,len(peticiones)):
            for (int j = petition_i+1; j < peticiones.getSize(); ++j) {
                // if peticiones[j] == pag
                peticiones.goToPos(j);
                cache->goToPos(pag_i);
                if (peticiones.getElement() == cache->getElement()) {
                    if (to_remove < j) {
                        to_remove = j;
                        to_remove_pag = pag_i;
                    }
                }
            }
        }
        //cache[to_remove_pag] = pe
        cache->goToPos(to_remove_pag);
        cache->remove();
        peticiones.goToPos(petition_i);
        cache->insert(peticiones.getElement());
        //fallos+=1
        ++fallos;

        printf("Cache: ");
        cache->print();
        printf("\n");
    }
    delete cache;
    printf("Fallos: %d\n", fallos);
    return fallos;
}

#endif // OPTIMAL_H
