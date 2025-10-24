#ifndef COMPUTER_H
#define COMPUTER_H

#include "MMU.h"
#include "utils.h"

class Parser {
  public:
    char* ops;
    int curr_op;
    FILE *f;
    

    Parser(int algorithm, int processes, int operations) {
      create_operations(processes, operations);
      f = fopen("../../tmp/data", "r");
      //optimal_mmu = new MMU(5);
    }

    ~Parser(){
      delete ops;
    }
  private:
    char buff[256];
    char token[256];
    char c;
    MMU *optimal_mmu;
    
    int parse_num(){
      int i = 0;
      while((c = fgetc(f)) != ',', c != ')'){
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
    }

    void parse_use(){
      int ptr = parse_num();
      optimal_mmu->use(ptr);
      // Guardarlo en algun lado para el optimo
    }

    void parse_function(){
      int i = 0;
      while((c = fgetc(f)) != '('){
        token[i++] = c;
      }
      token[i] = '\0';
      if (strcmp(token, "new") == 0){
        parse_new();
      } else if(strcmp(token, "use") == 0){
        parse_use();
      } else{
        printf("no hay\n");
      }

      fgetc(f); //skip al salto de linea

    }
    void next(){
      parse_function();
    }
};

#endif //COMPUTER_H
