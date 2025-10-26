#ifndef OPTIMAL_MMU_H
#define OPTIMAL_MMU_H

#include "../MMU.h"

class Optimal_MMU: public MMU {
public:
    std::vector<int> futureRequests;
    Optimal_MMU(std::vector<int> futureRequests){
        this->futureRequests = futureRequests;
    }
    int paging(Page to_insert_page) {
        int to_remove_index = -1;
        int farthest_use = -1;
        for (int i = 0; i < MEMORY_SIZE; ++i) {
            int next_use = -1;

            // se busca cuando se usara mas reciente
            for (int j = current_page+1; j < futureRequests.size(); ++j) {
                if (futureRequests[j] == memory[i]) {
                    next_use = j; 
                    break;
                }
            }
            if (next_use == -1) return i;

            // si el siguiente uso de esta pagina es mas lejano que el mas lejano actual
            // se selecciona este para usarlo
            if (next_use > farthest_use) {
                farthest_use = next_use;
                to_remove_index = i;
            }
        }
        return to_remove_index;
    }
        
};

#endif
