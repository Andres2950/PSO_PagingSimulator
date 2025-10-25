#ifndef PROCESS_H
#define PROCESS_H

#include "../data_structures/List.h"
#include "../data_structures/ArrayList.h"

class Process {
    public:
        static inline int pid_counter = 0;
        int pid;
        List<int>* pointers;

        Process(){
            pid = pid_counter;
            pid_counter++;
            pointers = new ArrayList<int>;
        }
        ~Process(){
            delete pointers;
        }

};

#endif //PROCESS_H
