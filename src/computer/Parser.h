#ifndef COMPUTER_H
#define COMPUTER_H

#include "MMU.h"
#include "algorithms/Optimal.h"
#include "utils.h"
#include <experimental/filesystem>

class Parser {
  public:
    Parser(int algorithm, int processes, int operations) {
      unsigned int seed = create_operations(processes, operations);
      ops.content = (char *) malloc(sizeof(char) * 4090);
      ops.pos = 0;
      ops.max_size = 4090;
      // TODO: agregar que use el archivo ingresado por el usuairo
      FILE *f = fopen("./tmp/data", "r");
      while ((c = fgetc(f)) != EOF) {
        ops.content[ops.pos++] = c;
        if(ops.pos == ops.max_size - 1){
          ops.max_size *= 2;
          ops.content = (char *) realloc(ops.content, ops.max_size);
        }
      }
      ops.content[ops.pos] = '\0';
      parse_for_optimal();
      printf("#########Paginas futuras#############\n");
      pages.print();
      printf("\n");
      optimal_mmu = new MMU(pages);
      other_mmu = new MMU(algorithm, 0);
      printf("Tipo %d\n", algorithm);
      int count = 1;
      while(ops.content[ops.pos] != '\0'){
        next();
        printf("############## CACHE OPS = %d################\n", count++);
        printf("--- Optimal ---\n");
        optimal_mmu->state.memory->print();
        printf("\ntime: %d\n", optimal_mmu->state.currentTime);
        printf("--- Other ---\n");
        other_mmu->state.memory->print();
        printf("\ntime: %d\n", other_mmu->state.currentTime);
        printf("\n###############################################\n");
      }
    }
    ~Parser(){
      if(ops.content)
        free(ops.content);
      delete optimal_mmu;
      delete other_mmu;
    }
  private:
    typedef struct{
      char *content;
      int pos;
      int max_size;
    } Operations;
    
    Operations ops;
    char buff[256];
    char token[256];
    char c;
    MMU *optimal_mmu;
    MMU *other_mmu;
    ArrayList<int> pages;
    ArrayList<int> ptr_pages;
    int page_id = 1, ptr_id = 1;

    void parse_for_optimal(){
      int i;
      int ptr_size, n_pages, pid, ptr, page_to_load;
      printf("Contenido:\n%s", ops.content);
      ops.pos = 0;
      while(ops.content[ops.pos] != '\0'){
        i = 0;
        while((c = ops.content[ops.pos++]) != '('){
          token[i++] = c;
        }
        token[i] = '\0';
        if (strcmp(token, "new") == 0){
          pid = parse_num();
          ptr_size = parse_num();
          n_pages = ((ptr_size % 4000) == 0 
                                        ? ptr_size / 4000 
                                        : ptr_size / 4000 + 1);
          for(int p = 0; p < n_pages; ++p){
            pages.append(page_id++);
            ptr_pages.append(ptr_id);
          }
          ptr_id++;
        } else if(strcmp(token, "use") == 0){
          ptr = parse_num();
          int ptr_start_pos = -1, ptr_end_pos;
          
          for(ptr_pages.goToStart(); !ptr_pages.atEnd(); ptr_pages.next()){
            if(ptr_pages.getElement() == ptr){
              if(ptr_start_pos == -1)
                ptr_start_pos = ptr_pages.getPos();
              else
                ptr_end_pos = ptr_pages.getPos();
            }
          }
          // copia de nuevo todas las paginas a cargar del puntero
          for(int i = ptr_start_pos; i <= ptr_end_pos; i++){
            pages.goToPos(i);
            page_to_load = pages.getElement();
            pages.append(page_to_load);
          }
        }
        while((c = ops.content[ops.pos++]) != '\n');
      }
      ops.pos = 0;
    }

    int parse_num(){
      int i = 0;
      while((c = ops.content[ops.pos++]) != ',' && c != ')'){
        buff[i++] = c;
      }
      buff[i] = '\0';
      i = atoi(buff);
      buff[0] = '\0';
      return i;
    }

    void parse_new(){
      int pid = parse_num();
      int size = parse_num();
      optimal_mmu->_new(pid, size);
      other_mmu->_new(pid, size);
    }

    void parse_use(){
      int ptr = parse_num();
      optimal_mmu->use(ptr);
      other_mmu->use(ptr);
    }

    void parse_delete(){
      int ptr = parse_num();
      optimal_mmu->_delete(ptr);
      other_mmu->_delete(ptr);
    }

    void parse_kill(){
      int pid = parse_num();
      optimal_mmu->kill(pid);
      other_mmu->kill(pid);
    }

    void parse_function(){
      int i = 0;
      while((c = ops.content[ops.pos++]) != '('){
        token[i++] = c;
      }
      token[i] = '\0';
      if (strcmp(token, "new") == 0){
        parse_new();
      } else if(strcmp(token, "use") == 0){
        parse_use();
      } else if(strcmp(token, "delete") == 0){
        parse_delete();
      } else if(strcmp(token, "kill") == 0){
        parse_kill();
      } else {
        printf("Error: Invalid token or format, found %s, expected 'new','use','delete' or 'kill'\n", token);
      }

      ops.pos++; //skip al salto de linea
    }
    void next(){
      parse_function();
    }
};

#endif //COMPUTER_H
