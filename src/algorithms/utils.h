#ifndef UTILS_H
#define UTILS_H

void create_operations();

#define UTILS_IMPLMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

unsigned int seed = 1760652336;

const char *file_path = "../../tmp/data";

int num_ops = 15;
int num_processes = 3;
enum options {
  NEW,
  USE,
  DELETE,
  KILL
};


typedef struct {
  int op;
  int arg;
} Operation;

typedef struct{
  int num_ops;
  int size_ops;
  Operation *ops;
  int *ptrs;
  int num_ptrs;
  int size_ptrs;
} Process;

Process **processes;

int get_random_ptr(Process *proc){
  return ((proc->num_ptrs > 0) ? (proc->ptrs[rand() % proc->num_ptrs]) : -1);
}

int delete_ptr(Process *proc){
  if(proc->num_ptrs == 0)
    return -1;
  int index = rand() % proc->num_ptrs;
  proc->num_ptrs--;
  int ptr = proc->ptrs[index];
  for(int i = index; i < proc->num_ptrs; i++){
    proc->ptrs[i] = proc->ptrs[i+1];
  }
  return ptr;
}

int ptr_count = 0;

typedef struct {
  int pid;
  int num_op;
} ProcRef;

typedef struct {
  char **text;
  ProcRef *proc_ref;
} Instr;

void create_operations(){
  srand(seed);
  processes = (Process **) malloc(sizeof(Process *) * num_processes);
  int standard_size = (num_ops / num_processes) + 2;
  Process *proc;
  int op, ptr;
  for(int p = 0; p < num_processes; ++p){
    processes[p] = (Process *) malloc(sizeof(Process));
    proc = processes[p];
    proc->ops = (Operation *) malloc(sizeof(Operation) * standard_size);
    proc->size_ops = standard_size;
    proc->ptrs = (int *) calloc((standard_size / 2) + 2, 4);
    proc->size_ptrs = (standard_size / 2) + 2;
    proc->num_ptrs = 0;
    proc->num_ops = 0;
  }

  int proc_i;
  Instr *instr = (Instr *) malloc(sizeof(Instr));
  instr->text = (char **) malloc(sizeof(char *) * num_ops);
  instr->proc_ref = (ProcRef *) malloc(sizeof(ProcRef) * num_ops);
  char buff[100];
  for(int op_i = 0; op_i < num_ops-num_processes; ++op_i){
    proc_i = rand() % num_processes;
    proc = processes[proc_i];
    op = rand() % 3;

    ptr = get_random_ptr(proc);

    if ((op == USE || op == DELETE) && (ptr == -1)) {
      op_i--;
      continue;
    }

    instr->text[op_i] = (char *) malloc(100);

    Operation new_op;
    switch(op){
      case NEW:
        ptr_count++;
        new_op.op = op;
        new_op.arg = ptr_count;
        proc->ops[proc->num_ops] = new_op;
        proc->ptrs[proc->num_ptrs] = ptr_count;
        
        sprintf(buff, "new(%d, %d)\n", proc_i + 1, (rand() % 100000) + 1);
        
        proc->num_ops++;
        proc->num_ptrs++;
        break;
      case USE:
        new_op.op = op;
        new_op.arg = ptr;
        proc->ops[proc->num_ops] = new_op;
          
        sprintf(buff, "use(%d)\n", ptr);
          
        proc->num_ops++;
        break;

      case DELETE:
        new_op.op = op;
        new_op.arg = ptr;
        proc->ops[proc->num_ops] = new_op;
        delete_ptr(proc);  
        sprintf(buff, "delete(%d)\n", ptr);
        

        proc->num_ops++;
        break;
      case KILL:
        new_op.op = op;
        new_op.arg = proc_i;
        proc->ops[proc->num_ops] = new_op;
        
        sprintf(buff, "kill(%d)\n", proc_i);
        
        proc->num_ops++;
        break;
    }
    strcpy(instr->text[op_i], buff);
    ProcRef proc_ref = {proc_i, proc->num_ops};
    instr->proc_ref[op_i] = proc_ref;
    strcpy(buff, "");

    if(proc->num_ops + 2 > proc->size_ops){
      proc->size_ops *= 2;
      proc->ops = (Operation *) realloc(proc->ops, sizeof(Operation) * proc->size_ops);
    } else if(proc->num_ptrs + 2 > proc->size_ops){
      proc->size_ptrs *= 2;
      proc->ptrs = (int *) realloc(proc->ptrs, sizeof(int) * proc->size_ptrs);
    }
  }
  FILE *f = fopen(file_path, "w");
 
  for(int op_i = 0; op_i < num_ops-num_processes; ++op_i){
    ProcRef p_ref = instr->proc_ref[op_i];
    proc = processes[p_ref.pid];
    if(p_ref.num_op == proc->num_ops){
      fprintf(f, instr->text[op_i]);
      fprintf(f, "kill(%d)\n", p_ref.pid + 1);
    } else {
      fprintf(f, instr->text[op_i]);
    }
  }
}

#endif // OPTIMAL_H
