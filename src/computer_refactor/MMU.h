#ifndef MMU_H
#define MMU_H

#include "../constants.h"
#include "Page.h"
#include <array>
#include <map>
#include <vector>

class MMU {
public:
  int page_id = 1, ptr_count = 1;
  int fault_time = 0;
  int type_algo;
  std::map<int, std::vector<int>> ptr_pageid_map;
  // TODO: lista enlazada
  std::map<int, std::vector<int>> process_ptrs_map;
  std::array<int, MEMORY_SIZE> memory;
  std::map<int, Page> disk;

  virtual int _new(int pid, int size) {
    int page_ammount =
        ((size % PAGE_SIZE == 0) ? size / PAGE_SIZE : size / PAGE_SIZE + 1);

    // Crear paginas nuevas
    std::vector<int> pages_id;
    for (int i = 0; i < page_ammount; i++) {
      Page page = Page(page_id++);
      page.l_addr = ptr_count;
      page.load_t = 0; // TODO:cambiar esto

      // Agregar en disco
      disk[page.id] = page;
      pages_id.push_back(page.id);

      // Agregar a RAM
      for (int j = 0; j < MEMORY_SIZE; j++) {
      }
    }

    return 1;
  }

  virtual void use(int ptr) = 0;

  virtual void _delete(int ptr) {
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
  }

  virtual void kill(int pid) {
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
