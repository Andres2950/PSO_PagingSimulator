#ifndef MMU_H
#define MMU_H

#include "Page.h"
#include "../data_structures/List.h"
#include "../data_structures/ArrayList.h"
#include "../data_structures/Dictionary.h"
#include "../data_structures/HashTable.h"
#include "algorithms/Algorithm.h"
#include "algorithms/FIFO.h"
#include "algorithms/Optimal.h"
#include "algorithms/SecondChance.h"
#include "../../constants.h"

enum ALGORITHM {
    FI_FO,
    SECOND_CHANCE,
    MRU,
    LRU,
    RANDOM
};

class MMU {
    public:
        int page_id = 1, ptr_count = 1;
        StatePerron state;
        //List<Page>* memory;
        //List<Page>* disk;
        Algorithm *algorithm;
        //no lista de enteros algo diferente
        //Table debe contener todos los punteros
        // es una Dictionary <ptr_id, ptr_pages>
        // Estoy usando un arraylist pq List necesita ser un puntero 
        // El problema de eso es que HashTable no sabe si K y V son punteros entonces no los libera
        // Entonces x eso uso el tipo concreto
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
                if (state.memory->getSize() == MEMORY_SIZE){
                  algorithm->execute(page, state);
                } else {
                  page.l_addr = ptr_count;
                  page.m_addr = state.memory->getSize();
                  page.is_loaded = 1;
                  page.load_t = 0;
                  state.currentTime += HIT_COST;
                  state.memory->append(page); 
                }
            }
            ptr_table->insert(ptr_count, list);
            ArrayList<int>* process_ptr;
            if (processes->contains(pid)){
              process_ptr = processes->getValue(pid);
              process_ptr->append(ptr_count);
              //processes->setValue(pid, process_ptr);
            } else {
              //TODO enlazada
              process_ptr = new ArrayList<int>(10);
              process_ptr->append(ptr_count);
              processes->insert(pid, process_ptr);
            }
            ptr_count++;
            return ptr_count-1;
        }

        void use(int ptr){
            ArrayList<Page>* pages = ptr_table->getValue(ptr);
            for (pages->goToStart(); !pages->atEnd(); pages->next()){
                Page page = pages->getElement();
                // TODO: validar
                algorithm->execute(page, state);
            }
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
        }

        void kill(int pid){
          if(processes->contains(pid)){
            ArrayList<int> *process_ptrs = processes->getValue(pid);
            for(process_ptrs->goToStart(); !process_ptrs->atEnd(); process_ptrs->next()){
              _delete(process_ptrs->getElement());
            }
            processes->remove(pid);
          }
        }

        MMU(){}

        MMU(ALGORITHM algorithm){
            //Memoria real de 100 paginas
           switch (algorithm){
              case FI_FO: 
                break;
              case SECOND_CHANCE:
                break;
              case MRU:
                break;
              case LRU:
                break;
              case RANDOM:
                break;
           }
           state.currentTime = 0;
           state.memory = new ArrayList<Page>(100); 
           state.disk = new ArrayList<Page>();
           ptr_table = new HashTable<int, ArrayList<Page>*>();
           processes = new HashTable<int, ArrayList<int>*>();
        }
        MMU(ArrayList<int> future) {
            algorithm = new Optimal(future);
            state.memory = new ArrayList<Page>(100);
            state.disk = new ArrayList<Page>();
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
