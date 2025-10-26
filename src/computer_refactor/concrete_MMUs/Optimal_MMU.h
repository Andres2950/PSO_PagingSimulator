#ifndef OPTIMAL_MMU_H
#define OPTIMAL_MMU_H

#include "../MMU.h"
#include <algorithm>

class Optimal_MMU : public MMU {
public:
  std::vector<int> futureRequests;
  Optimal_MMU(std::vector<int> futureRequests) {
    this->futureRequests = futureRequests;
  }
  int paging(Page to_insert_page) {
    int to_remove_index = -1;
    for (int pag_i = 0; pag_i < futureRequests.size(); ++pag_i) {
      std::vector<int> tmp;
      // tmp = future[current_page:]
      for (int i = current_page; i < futureRequests.size(); ++i) {
        tmp.push_back(futureRequests[i]);
      }
      // if pag_i not in tmp
      int count = std::count(tmp.begin(), tmp.end(), pag_i);
      if (count == 0) {
        to_remove_index = pag_i;
        break; // si no esta, simplemente se puede meter ahi
      }
      // for i in range(current_page+1,len(future))
      for (int j = current_page + 1; j < futureRequests.size(); ++j) {
        if (futureRequests[j] == memory[pag_i]) {
          if (to_remove_index < j) {
            to_remove_index = j;
          }
        }
      }
    }
    return to_remove_index;
  }
};

#endif
