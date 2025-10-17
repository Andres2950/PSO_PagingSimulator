#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
int main(){
  unsigned int seed = create_operations(3, 15);
  printf("seed: %u\n", seed);
  return 0;
}
