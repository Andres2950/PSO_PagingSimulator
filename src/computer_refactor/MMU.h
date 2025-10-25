#ifndef MMU_H
#define MMU_H

#include "../constants.h"
#include "Page.h"
#include <array>
#include <map>
#include <vector>

class MMU {
public:
  int page_id_counter = 1, ptr_count = 1;
  int fault_time = 0;
  int time = 0;
  int type_algo;
  std::map<int, std::vector<int>> ptr_pageid_map;
  std::map<int, std::vector<int>> process_ptrs_map;
  std::array<int, MEMORY_SIZE> memory;
  std::map<int, Page> disk;

  void update_times(){
    for(int i = 0; i < MEMORY_SIZE; ++i){
        Page page = disk.at(memory[i]);
        page.load_t = time - page.timestamp;
    }
  }

  virtual int paging(Page to_insert_page)=0;
  int _new(int pid, int size) {
    int page_ammount =
        ((size % PAGE_SIZE == 0) ? size / PAGE_SIZE : size / PAGE_SIZE + 1);

    // Crear paginas nuevas
    std::vector<int> pages_id;
    for (int i = 0; i < page_ammount; i++) {
      Page page = Page(page_id_counter++);
      page.l_addr = ptr_count;
      page.timestamp = time; // TODO:cambiar esto

      // Agregar en disco
      disk[page.id] = page;
      pages_id.push_back(page.id);

      // Agregar a RAM
      bool added = false;
      for (int j = 0; j < MEMORY_SIZE; j++) {
          if (memory[j] == -1) {
            page.m_addr = j;
            memory[j] = page.id;
            added = true;
            time += HIT_COST;
            break;
          }
      }
      //paginar
      if (!added) {
          int to_remove = paging(page);
          memory[to_remove] = page.id;
          time += FAULT_COST;
          fault_time += FAULT_COST;
      }
    }

    process_ptrs_map.insert(ptr_count, pages_id);
    
    ptr_count++;
    update_times();

    return ptr_count-1;
  }

  void use(int ptr){
    //se obtienen los ids
    std::vector<int> pages = ptr_pageid_map.at(ptr);
    for(int i = 0; i < pages.size(); i++){
        // Revisa si ya esta en RAM 
        Page page = disk.at(pages[i]);
        bool added = false;
        for (int j = 0; j < MEMORY_SIZE; j++) {
            if (memory[j] == page.id) {
                added = true;
                time += HIT_COST;
                break;
            }
        }
        if (added) continue;
        // Revisar si hay espacio en la ram 
        for (int j = 0; j < MEMORY_SIZE; j++) {
          if (memory[j] == -1) {
            page.m_addr = j;
            memory[j] = page.id;
            added = true;
            time += HIT_COST;
            break;
          }
        }
        // Si no hay espacio paginar
        if (!added) {
          int to_remove = paging(page);
          memory[to_remove] = page.id;
          time += FAULT_COST;
          fault_time += FAULT_COST;
        }
    }
    update_times();
  }

  void _delete(int ptr) {
    // Obtener page ids asociados con el puntero
    std::vector<int> pageIds = ptr_pageid_map.at(ptr);

    // Borrar paginas
    for (int i = 0; i < pageIds.size(); i++) {
      Page page = disk.at(pageIds[i]);
      // Sacar pagina de RAM
      if (page.is_loaded) {
        memory[page.m_addr] = -1;
      }
      disk.erase(pageIds[i]);
    }

    // Borrar puntero
    ptr_pageid_map.erase(ptr);
    // No se borra de proccess_ptrs_map porque costaria buscarlo pero creo que
    // no da problemas
    time += HIT_COST;
    update_times();
  }

  void kill(int pid) {
    // Obtener los punteros asociados al vector
    std::vector ptrs = process_ptrs_map.at(pid);

    // Hacerle delete a los punteros
    std::map<int, std::vector<int>>::iterator it;
    for (int i = 0; i < ptrs.size(); i++) {
      it = ptr_pageid_map.find(ptrs[i]);
      if (it != ptr_pageid_map.end()) {
        _delete(ptrs[i]);
      }
    }
    // Borrar el proceso
    process_ptrs_map.erase(pid);
  }
};

#endif
