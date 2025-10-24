#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
int main(){
  unsigned int seed = create_operations(101, 1000);
  printf("seed: %u\n", seed);
  return 0;
}
