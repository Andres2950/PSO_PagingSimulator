#include <stdio.h>
#include <string>

#include "computer/algorithms/Algorithm.h"
#include "computer/algorithms/Optimal.h"
#include "computer/algorithms/FIFO.h"
#include "computer/algorithms/SecondChance.h"
#include "constants.h"
#include "data_structures/List.h"
#include "data_structures/ArrayList.h"
#include "computer/Page.h"

using std::string;

void hello_world(string s){
    printf("Hello World!\n");
}



int main(int argc, char* argv[]){
    hello_world("print");

    List<Page>* requests = new ArrayList<Page>();
    ArrayList<int> otherRequests = ArrayList<int>(); 
    
    //peticiones = [3,3,4,8,5,9,3,2,5,4]
    requests->append(Page(3));
    otherRequests.append(3);
    requests->append(Page(3));
    otherRequests.append(3);
    requests->append(Page(4));
    otherRequests.append(4);
    requests->append(Page(8));
    otherRequests.append(8);
    requests->append(Page(5));
    otherRequests.append(5);
    requests->append(Page(9));
    otherRequests.append(9);
    requests->append(Page(3));
    otherRequests.append(3);
    requests->append(Page(2));
    otherRequests.append(2);
    requests->append(Page(5));
    otherRequests.append(5);
    requests->append(Page(4));
    otherRequests.append(4);
    
    int k = 2;
    
    printf("### OPTIMAL ###\n");
    StatePerron state;
    state.currentTime = 0;
    state.to_insert_i = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    state.memory->goToStart();
    requests->goToStart();
    otherRequests.goToStart();
    Algorithm* optimal = new Optimal(otherRequests);
    /*while(!requests->atEnd()){
        state.to_insert_i++;
        optimal->execute(requests->getElement(), state);
        requests->next();
    }*/
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;
   
    printf("### FIFO ###\n");
    state.currentTime = 0;
    state.to_insert_i = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    state.memory->goToStart();
    requests->goToStart();
    printf("Size: %d\n", requests->getSize());
    Algorithm* fifo = new FIFO();
    while(!requests->atEnd()){
        state.to_insert_i++;
        if (state.memory->getSize() < MEMORY_SIZE) {state.memory->append(requests->getElement());}
        fifo->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;
    
    printf("### SECOND CHANCE ###\n");
    state.currentTime = 0;
    state.to_insert_i = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    state.memory->goToStart();
    requests->goToStart();
    Algorithm* second_chance = new SecondChance();
    while(!requests->atEnd()){
        state.to_insert_i++;
        if (state.memory->getSize() < MEMORY_SIZE) {state.memory->append(requests->getElement());}
        second_chance->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;


    printf("### MRU ###\n");
    state.currentTime = 0;
    state.to_insert_i = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    state.memory->goToStart();
    requests->goToStart();
    Algorithm* mru = new SecondChance();
    while(!requests->atEnd()){
        state.to_insert_i++;
        if (state.memory->getSize() < MEMORY_SIZE) {state.memory->append(requests->getElement());}
        mru->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;
    return 0;
}
