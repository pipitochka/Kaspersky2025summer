#include "hash.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

//function to hash file by sum of bytes
unsigned int hash(const char* path){
  unsigned int result = 0;
    
  int t = 0;
  
  FILE *f = fopen(path, "rb");
  if (f == NULL) {
    printf("Error opening file: %s\n", path);
    return 0; 
  }
  
  unsigned char c;
  
  while (fread(&c, sizeof(c), 1, f) == 1) {
    result += c;
  }
  
  fclose(f);
  
  return result;
}