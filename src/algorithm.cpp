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
    List<Page>* otherRequests = new ArrayList<Page>();
    
    //peticiones = [3,3,4,8,5,9,3,2,5,4]
    requests->append(Page(3));
    otherRequests->append(Page(3));
    requests->append(Page(3));
    otherRequests->append(Page(3));
    requests->append(Page(4));
    otherRequests->append(Page(4));
    requests->append(Page(8));
    otherRequests->append(Page(8));
    requests->append(Page(5));
    otherRequests->append(Page(5));
    requests->append(Page(9));
    otherRequests->append(Page(9));
    requests->append(Page(3));
    otherRequests->append(Page(3));
    requests->append(Page(2));
    otherRequests->append(Page(2));
    requests->append(Page(5));
    otherRequests->append(Page(5));
    requests->append(Page(4));
    otherRequests->append(Page(4));
    
    int k = 2;
    
    printf("### OPTIMAL ###\n");
    StatePerron state;
    state.currentTime = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    //Inserta las cosas hasta que se llene el cache
    state.memory->goToStart();
    requests->goToStart();
    for (int i = 0; i < k; ++i) {
        state.memory->append(requests->getElement());
        state.to_insert_i = i;
        requests->next();
        state.currentTime += FAULT_COST;
    }
    printf("Starting cache: ");
    state.memory->print();
    printf("\n");

    Algorithm* optimal = new Optimal(otherRequests);
    while(!requests->atEnd()){
        state.to_insert_i++;
        optimal->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;
   
    printf("### FIFO ###\n");
    state.currentTime = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    //Inserta las cosas hasta que se llene el cache
    state.memory->goToStart();
    requests->goToStart();
    for (int i = 0; i < k; ++i) {
        state.memory->append(requests->getElement());
        state.to_insert_i = i;
        requests->next();
        state.currentTime += FAULT_COST;
    }
    printf("Starting cache: ");
    state.memory->print();
    printf("\n");

    Algorithm* fifo = new FIFO();
    while(!requests->atEnd()){
        state.to_insert_i++;
        fifo->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;
    
    printf("### SECOND CHANCE ###\n");
    state.currentTime = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);
    //Inserta las cosas hasta que se llene el cache
    state.memory->goToStart();
    requests->goToStart();
    for (int i = 0; i < k; ++i) {
        state.memory->append(requests->getElement());
        state.to_insert_i = i;
        requests->next();
        state.currentTime += FAULT_COST;
    }
    printf("Starting cache: ");
    state.memory->print();
    printf("\n");

    Algorithm* second_chance = new SecondChance();
    while(!requests->atEnd()){
        state.to_insert_i++;
        second_chance->execute(requests->getElement(), state);
        requests->next();
    }
    printf("Time: %d\n", state.currentTime);
    delete state.memory;
    delete state.disk;

    return 0;
}
