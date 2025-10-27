#ifndef COMPUTER_H
#define COMPUTER_H

#include "MMU.h"
#include "concrete_MMUs/FIFO_MMU.h"
#include "concrete_MMUs/LRU_MMU.h"
#include "concrete_MMUs/MRU_MMU.h"
#include "concrete_MMUs/Optimal_MMU.h"
#include "concrete_MMUs/Random_MMU.h"
#include "concrete_MMUs/SecondChance_MMU.h"
#include <cstdio>
#include <cstring>

class Parser {
public:
  MMU *optimal_mmu;
  MMU *other_mmu;
  int current_op = 1;
  int totalProcesses = 0;
  Parser(int algorithm, const char *filepath) {
    ops.content = (char *)malloc(sizeof(char) * 4090);
    ops.pos = 0;
    ops.max_size = 4090;
    // TODO: agregar que use el archivo ingresado por el usuairo
    FILE *f = fopen(filepath, "r");
    while ((c = fgetc(f)) != EOF) {
      ops.content[ops.pos++] = c;
      if (ops.pos == ops.max_size - 1) {
        ops.max_size *= 2;
        ops.content = (char *)realloc(ops.content, ops.max_size);
      }
    }
    ops.content[ops.pos] = '\0';
    totalProcesses = getTotalProcesses();
    parse_for_optimal();
    // printf("#########Paginas futuras#############\n");
    // pages.print();
    printf("\n");
    optimal_mmu = new Optimal_MMU(pages);
    switch (algorithm) {
    case ALG_FIFO:
      other_mmu = new FIFO_MMU();
      printf("Crea fifo\n");
      break;
    case ALG_SECOND_CHANCE:
      other_mmu = new Second_Chance_MMU();
      break;
    case ALG_MRU:
      other_mmu = new MRU_MMU();
      break;
    case ALG_LRU:
      other_mmu = new LRU_MMU();
      break;
    case ALG_RANDOM:
      other_mmu = new Random_MMU();
      break;
    }
    /*//other_mmu = new MMU(algorithm, 0);
    printf("Tipo %d\n", algorithm);
    int count = 1;
    while(ops.content[ops.pos] != '\0'){
      next();
      printf("############## CACHE OPS = %d################\n", count++);
      printf("--- Optimal ---\n");
      //optimal_mmu->state.memory->print();
      printf("\ntime: %d\n", optimal_mmu->time);
      printf("--- Other ---\n");
      //other_mmu->state.memory->print();
      printf("\ntime: %d\n", other_mmu->time);
      printf("\n###############################################\n");
    }*/
  }
  ~Parser() {
    if (ops.content)
      free(ops.content);
    delete optimal_mmu;
    delete other_mmu;
  }
  bool executeInstruction() {
    printf("############## NEXT()################\n");
    next();
    printf("############## CACHE OPS = %d################\n", current_op++);
    printf("--- Optimal ---\n");
    for (int i = 0; i < MEMORY_SIZE; ++i) {
      printf("%d \t", optimal_mmu->memory[i]);
      if (i % 10 == 0)
        printf("\n");
    }
    printf("\n--- FIFO ---\n");

    for (int i = 0; i < MEMORY_SIZE; ++i) {
      printf("%d \t", other_mmu->memory[i]);
      if ((i + i) % 10 == 0)
        printf("\n");
    }
    return ops.content[ops.pos] != '\0';
  }

private:
  typedef struct {
    char *content;
    int pos;
    int max_size;
  } Operations;

  Operations ops;
  char buff[256];
  char token[256];
  char c;
  std::vector<int> pages;
  std::vector<int> ptr_pages;
  int page_id = 1, ptr_id = 1;

  void parse_for_optimal() {
    int i;
    int ptr_size, n_pages, pid, ptr, page_to_load;
    ops.pos = 0;
    while (ops.content[ops.pos] != '\0') {
      i = 0;
      while ((c = ops.content[ops.pos++]) != '(') {
        token[i++] = c;
      }
      token[i] = '\0';
      if (strcmp(token, "new") == 0) {
        pid = parse_num();
        ptr_size = parse_num();
        n_pages = ((ptr_size % PAGE_SIZE) == 0 ? ptr_size / PAGE_SIZE
                                               : ptr_size / PAGE_SIZE + 1);
        for (int p = 0; p < n_pages; ++p) {
          pages.push_back(page_id++);
          ptr_pages.push_back(ptr_id);
        }
        ptr_id++;
      } else if (strcmp(token, "use") == 0) {
        ptr = parse_num();
        int ptr_start_pos = -1, ptr_end_pos;

        for (int j = 0; j < ptr_pages.size(); ++j) {
          if (ptr_pages[j] == ptr) {
            if (ptr_start_pos == -1) {
              ptr_start_pos = j;
              ptr_end_pos = j;
            } else
              ptr_end_pos = j;
          }
        }
        // copia de nuevo todas las paginas a cargar del puntero
        for (int j = ptr_start_pos; j <= ptr_end_pos; ++j) {
          page_to_load = pages[j];
          pages.push_back(page_to_load);
        }
      }
      while ((c = ops.content[ops.pos++]) != '\n' && c != '\0')
        ;
    }
    ops.pos = 0;
    /*for(int i = 0; i < pages.size(); i++){
      printf("%d \t", pages[i]);
      if(i%10 == 0)
        printf("\n");
    }*/
  }

  int parse_num() {
    int i = 0;
    while ((c = ops.content[ops.pos++]) != ',' && c != ')') {
      buff[i++] = c;
    }
    buff[i] = '\0';
    i = atoi(buff);
    buff[0] = '\0';
    return i;
  }

  void parse_new() {
    int pid = parse_num();
    int size = parse_num();
    optimal_mmu->_new(pid, size);
    other_mmu->_new(pid, size);
  }

  void parse_use() {
    int ptr = parse_num();
    optimal_mmu->use(ptr);
    other_mmu->use(ptr);
  }

  void parse_delete() {
    int ptr = parse_num();
    optimal_mmu->_delete(ptr);
    other_mmu->_delete(ptr);
  }

  void parse_kill() {
    int pid = parse_num();
    optimal_mmu->kill(pid);
    other_mmu->kill(pid);
  }

  void parse_function() {
    int i = 0;
    while ((c = ops.content[ops.pos++]) != '(') {
      token[i++] = c;
    }
    token[i] = '\0';
    if (strcmp(token, "new") == 0) {
      parse_new();
    } else if (strcmp(token, "use") == 0) {
      parse_use();
    } else if (strcmp(token, "delete") == 0) {
      parse_delete();
    } else if (strcmp(token, "kill") == 0) {
      parse_kill();
    } else {
      printf("Error: Invalid token or format, found %s, expected "
             "'new','use','delete' or 'kill'\n",
             token);
    }

    ops.pos++; // skip al salto de linea
  }
  void next() { parse_function(); }

  int getTotalProcesses() {
    if (ops.content == nullptr) {
      return 0;
    }

    int i = 0;
    int total = 0;
    while (ops.content[i] != '\0') {
      if (ops.content[i] == 'k') {
        total++;
      }
      i++;
    }

    return total;
  }
};

#endif // COMPUTER_H
