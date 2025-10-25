#ifndef MMU_H
#define MMU_H

#include <stdio.h>
#include "Page.h"
#include "../data_structures/List.h"
#include "../data_structures/ArrayList.h"
#include "../data_structures/Dictionary.h"
#include "../data_structures/HashTable.h"
#include "algorithms/Algorithm.h"
#include "algorithms/FIFO.h"
#include "algorithms/Optimal.h"
#include "algorithms/SecondChance.h"
#include "algorithms/MRU.h"
#include "algorithms/LRU.h"
#include "algorithms/Random.h"
#include "../../constants.h"

#define FI_FO 0
#define SECOND_CHANCE 1
#define _MRU 2
#define _LRU 3
#define RANDOM 4
#define OPTIMAL 5

class MMU {
    public:
        int page_id = 1, ptr_count = 1;
        int fault_time = 0;
        int type_algo;
        StatePerron state;
        Algorithm *algorithm;
        Dictionary<int, ArrayList<Page>*>* ptr_table;
        // TODO: lista enlazada
        Dictionary<int, ArrayList<int>*>* processes;

        int _new(int pid, int size){
            state.memory->getSize();
            int page_ammount = ((size % PAGE_SIZE == 0) 
                                                ? size/PAGE_SIZE 
                                                : size/PAGE_SIZE+1);

            ArrayList<Page>* list = new ArrayList<Page>();       //Lista de paginas del ptr
            for (int i = 0; i < page_ammount; i++) {
                Page page = Page(page_id++);
                list->append(page);
                assert(state.memory != nullptr);
                if (state.memory->getSize() > MEMORY_SIZE){
                  int t = algorithm->execute(page, state);
                  //printf("EXECUTE\n");
                  if (t == FAULT_COST) fault_time += t;
                  state.currentTime += t;
                } else {
                  page.m_addr = state.memory->getSize();
                  page.is_loaded = 1;
                  page.load_t = 0;
                  if (type_algo == _MRU || type_algo == _LRU) {
                    page.mark = state.currentTime;
                  }
                  state.currentTime += HIT_COST;
                  state.memory->append(page); 
                }
                page.l_addr = ptr_count;
            }
            ptr_table->insert(ptr_count, list);
            ArrayList<int>* process_ptr;
            if (processes->contains(pid)){
              process_ptr = processes->getValue(pid);
              process_ptr->append(ptr_count);
              //processes->setValue(pid, process_ptr);
            } else {
              process_ptr = new ArrayList<int>(10);
              process_ptr->append(ptr_count);
              processes->insert(pid, process_ptr);
            }
            ptr_count++;
            //printf("new %d: %d\n", type_algo, state.currentTime);            
            return ptr_count-1;
        }

        void use(int ptr){
            ArrayList<Page>* pages = ptr_table->getValue(ptr);
            for (pages->goToStart(); !pages->atEnd(); pages->next()){
                Page page = pages->getElement();
                if (state.memory->getSize() <= MEMORY_SIZE) {
                    page.m_addr = state.memory->getSize();
                    page.is_loaded = 1;
                    page.load_t = 0;
                    if (type_algo == _MRU || type_algo == _LRU) {
                        page.mark = state.currentTime;
                    }
                    state.currentTime += HIT_COST;
                    state.memory->append(page);
                } else {
                    int t = algorithm->execute(page, state);
                    //printf("EXECUTE\n");
                    if (t == FAULT_COST) fault_time += t;
                    state.currentTime += t;
                }
            }
            //printf("use %d: %d\n", type_algo, state.currentTime);            
        }

        void _delete(int ptr) {
          ArrayList<Page>* pages = ptr_table->getValue(ptr);
          int index;
          Page page;
          for (pages->goToStart(); !pages->atEnd(); pages->next()){
            page = pages->getElement();
            if ((index = state.memory->indexOf(page)) != -1) {
              state.memory->goToPos(index);
              state.memory->remove();
            } else {
              index = state.disk->indexOf(page);
              state.disk->goToPos(index);
              state.disk->remove();
            }
            state.currentTime += HIT_COST;
          }
          ptr_table->remove(ptr);
          ArrayList<int>* pids = (ArrayList<int> *)processes->getKeys();
          ArrayList<int> *ptrs;

          int pos, pid;
          for(pids->goToStart(); !pids->atEnd(); pids->next()){
            pid = pids->getElement();
            ptrs = processes->getValue(pid);
            pos = ptrs->indexOf(ptr);
            if(pos != -1){
              ptrs->goToPos(pos);
              ptrs->remove();
            }
          }
          //printf("delete %d: %d\n", type_algo, state.currentTime);            
        }

        void kill(int pid){
          if(processes->contains(pid)){
            ArrayList<int> *process_ptrs = processes->getValue(pid);
            for(process_ptrs->goToStart(); !process_ptrs->atEnd(); process_ptrs->next()){
              _delete(process_ptrs->getElement());
            }
            processes->remove(pid);
          }
          //printf("kill %d: %d\n", type_algo, state.currentTime);            
        }

        MMU(int algorithm, int xd){
            //Memoria real de 100 paginas
           switch (algorithm){
              case FI_FO: 
                this->algorithm = new FIFO();
                printf("MMU TYPE FIFO\n");
                break;
              case SECOND_CHANCE:
                this->algorithm = new SecondChance();
                printf("MMU TYPE SECOND CHANCE\n");
                break;
              case _MRU:
                this->algorithm = new MRU();
                printf("MMU TYPE MRU\n");
                break;
              case _LRU:
                this->algorithm = new LRU();
                printf("MMU TYPE LRU\n");
                break;
              case RANDOM:
                this->algorithm = new Random();
                printf("MMU TYPE RANDOM\n");
                break; 
           }
           type_algo = algorithm;
           state.memory = new ArrayList<Page>(100); 
           state.disk = new ArrayList<Page>();
           state.currentTime = 0;
           ptr_table = new HashTable<int, ArrayList<Page>*>();
           processes = new HashTable<int, ArrayList<int>*>();
        }
        MMU(ArrayList<int> future) {
            printf("MMU TYPE OPTIMAl\n");
            algorithm = new Optimal(future);
            type_algo = OPTIMAL;
            state.memory = new ArrayList<Page>(100);
            state.disk = new ArrayList<Page>();
            state.currentTime = 0;
            ptr_table = new HashTable<int, ArrayList<Page>*>();
            processes = new HashTable<int, ArrayList<int>*>();
        }

        ~MMU() {
            delete state.memory;
            delete state.disk;
            delete ptr_table;
            delete processes;
        }
};


#endif
