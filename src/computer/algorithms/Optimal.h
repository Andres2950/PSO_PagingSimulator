#ifndef OPTIMAL_H
#define OPTIMAL_H

#include <stdio.h>
#include "Algorithm.h"
#include "../Page.h"
#include "../../data_structures/List.h"
#include "../../data_structures/ArrayList.h"
#include "../../constants.h"

class Optimal: public Algorithm {
    private:
        // No esoty seguro de como hacer con esto
        // Por ahora voy a dejarlo como una lista de Page
        // Pero deberia ser posible con una lista de int
        List<Page>* futureRequests;
    public:
        Optimal(List<Page>* futureRequests){
            this->futureRequests = futureRequests;
        }
        // Solo para insertar una pagina, supone que la RAM ya esta llena
        // Supone que el i dentro de la lista de las páginas es el mismo que el id
        void execute(Page to_insert, StatePerron& state) {
            int petition_i = state.to_insert_i;
            futureRequests->goToPos(petition_i);
            // if pe in cache
            if (state.memory->contains(futureRequests->getElement())) {
                printf("HIT: %d\n", futureRequests->getElement().id);
                state.currentTime += HIT_COST;
                return;
            }
    
            int to_remove_pag = 0;
            int to_remove = -1;
            //for pag_i, pag in enumerate(cache)
            for (int pag_i = 0; pag_i < state.memory->getSize(); ++pag_i) {
                List<int>* tmp = new ArrayList<int>();
                // tmp = peticiones[pe_i:]
                for (int i = petition_i; i < futureRequests->getSize(); ++i) {
                    futureRequests->goToPos(i);
                    tmp->append(futureRequests->getElement().id); 
                }
                // if pag not int peticiones[pe_i:]
                state.memory->goToPos(pag_i);
                if (!tmp->contains(state.memory->getElement().id)){
                    to_remove_pag = pag_i;
                    break;
                }
                delete tmp;
                // for j in range(pe_i+1,len(peticiones)):
                for (int j = petition_i+1; j < futureRequests->getSize(); ++j) {
                    // if peticiones[j] == pag
                    futureRequests->goToPos(j);
                    state.memory->goToPos(pag_i);
                    if (futureRequests->getElement().id == state.memory->getElement().id) {
                        if (to_remove < j) {
                            to_remove = j;
                            to_remove_pag = pag_i;
                        }
                    }
                }
            }
            //cache[to_remove_pag] = peticiones
            state.memory->goToPos(to_remove_pag);
            Page removed = state.memory->remove();
            state.disk->append(removed);
            state.currentTime += FAULT_COST;
            futureRequests->goToPos(petition_i);
            state.memory->insert(futureRequests->getElement());
            
            printf("Cache:");
            state.memory->print();
            printf("\n");
        }
};

#endif //OPTIMAL_H
