#ifndef COMPUTER_H
#define COMPUTER_H

#include "MMU.h"

class Computer {
    public:
        MMU* mmu;

        Computer() {
            mmu = new MMU();
        }
        ~Computer(){
            delete mmu;
        }

};

#endif //COMPUTER_H
