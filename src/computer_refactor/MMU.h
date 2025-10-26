#ifndef MMU_H
#define MMU_H

#include "../constants.h"
#include "Page.h"
#include <array>
#include <experimental/filesystem>
#include <map>
#include <vector>

class MMU {
public:
  int page_id_counter = 1, ptr_count = 1;
  int current_page = 0;
  int fault_time = 0;
  int time = 0;
  int type_algo;
  std::map<int, std::vector<int>> ptr_pageid_map;
  std::map<int, std::vector<int>> process_ptrs_map;
  std::array<int, MEMORY_SIZE> memory;
  std::map<int, Page> disk;

  virtual int paging(Page to_insert_page) = 0;
  virtual void mark_used(Page page){}
  virtual void mark_inRAM(Page page){}

  void update_times() {
    for (int i = 0; i < MEMORY_SIZE; ++i) {
      Page page = disk.at(memory[i]);
      page.load_t = time - page.timestamp;
    }
  }


  //TODO: esto tira IOT de un map::at, pero no se donde esta eso
  int _new(int pid, int size) {
    printf("new\n");
    int page_ammount =
        ((size % PAGE_SIZE == 0) ? size / PAGE_SIZE : size / PAGE_SIZE + 1);

    // Crear paginas nuevas
    std::vector<int> pages_id;
    for (int i = 0; i < page_ammount; i++) {
      Page page = Page(page_id_counter++);
      page.l_addr = ptr_count;

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
      // paginar
      if (!added) {
        int to_remove = paging(page);
        memory[to_remove] = page.id;
        time += FAULT_COST;
        fault_time += FAULT_COST;
      }
      mark_used(page);
      page.timestamp = time;
      ++current_page;
    }

    // Asociar proceso a ptr
    process_ptrs_map[pid].push_back(ptr_count);
    // Asociar ptr a paginas
    ptr_pageid_map[ptr_count] = pages_id;

    ptr_count++;
    update_times();
    printf("new end\n");
    return ptr_count - 1;
  }

  void use(int ptr) {
    printf("use\n");
    std::vector<int> pageIds = ptr_pageid_map.at(ptr);
    bool added;
    for (int i = 0; i < pageIds.size(); i++) {
      Page page = disk.at(pageIds[i]);
      if (page.is_loaded) {
        time += HIT_COST;
        mark_inRAM(page);
      } else {
        added = false;
        for (int j = 0; j < MEMORY_SIZE; j++) {
          if (memory[j] == -1) {
            page.m_addr = j;
            memory[j] = page.id;
            time += HIT_COST;
            added = true;
            page.timestamp = time;
            break;
          }
        }
        if (!added) {
          int to_remove = paging(page);
          memory[to_remove] = page.id;
          page.timestamp = time;
          time += FAULT_COST;
          fault_time += FAULT_COST;
        }
        mark_used(page);
        page.is_loaded = true;
        }
      ++current_page;
    }
    update_times();
    printf("use end\n");
  };

  void _delete(int ptr) {
    printf("delete\n");
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
    printf("delete end\n");
  }

  void kill(int pid) {
    printf("kill\n");
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
    printf("kill end\n");
  }
};

#endif
