#include <stdio.h>
#include <string>

#include "computer/algorithms/Algorithm.h"
#include "computer/algorithms/Optimal.h"
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
    
    //peticiones = [3,1,4,8,5,9,3,2,5,4]
    requests->append(Page(3, 0, 0, 0));
    requests->append(Page(1, 0, 0, 0));
    requests->append(Page(4, 0, 0, 0));
    requests->append(Page(8, 0, 0, 0));
    requests->append(Page(5, 0, 0, 0));
    requests->append(Page(9, 0, 0, 0));
    requests->append(Page(3, 0, 0, 0));
    requests->append(Page(2, 0, 0, 0));
    requests->append(Page(5, 0, 0, 0));
    requests->append(Page(4, 0, 0, 0));
    
    printf("### OPTIMAL ###\n");
    StatePerron state;
    state.currentTime = 0;
    state.disk = new ArrayList<Page>();
    state.memory = new ArrayList<Page>(2);

    int k = 2;

    //Inserta las cosas hasta que se llene el cache
    state.memory->goToStart();
    for (int i = 0; i < k; ++i) {
        state.memory->append(requests->getElement());
        state.to_insert_i = i;
        requests->next();
    }

    printf("Starting cache: ");
    state.memory->print();
    printf("\n");

    Algorithm* optimal = new Optimal(requests);

    requests->goToStart();
    while(!requests->atEnd()){
        state.to_insert_i++;
        optimal->execute(requests->getElement(), state);
        requests->next();
    }


    delete state.memory;
    delete state.disk;
    return 0;
}
