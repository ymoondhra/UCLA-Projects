#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int IOErrorCheck(void) {
  if(ferror(stdin)) {
    fprintf(stderr, "There is an I/O issue. Leaving program now. \n");
    return 0;
  }
  return 1;
}

int compare(const void* x, const void* y) {
    char const first = *(const char *) x;
    char const second = *(const char *) y;
    if(first > second) {
      return 1;
    }
    else if(first == second) {
      return 0;
    }
    else {
      return -1;
    }
}

int main(int argc, char **argv) {
  //printf("TESTING");
  //if(argc < 2) {
  //  fprintf(stderr, "Error: need two arguments of the same length. Exiting...");
  //  exit(1);
  //}

  char* from = argv[1];
  char* to = argv[2];


  if(strlen(from) != strlen(to)) {
    fprintf(stderr, "Error: the decrypted string did not match the encrypted string");
    exit(1);
  }
  
  char* sortedfrompointer = strdup(from);
  qsort(sortedfrompointer, strlen(sortedfrompointer), sizeof(char), compare);
  for(int i = 0; i < strlen(sortedfrompointer) - 1; i++) {
    if(sortedfrompointer[i] == sortedfrompointer[i+1]) {
      fprintf(stderr, "duplicate bytes within the first argument. Exiting \n");
      exit(1);
    }
  }
  free(sortedfrompointer);
  
  char c[1];
  ssize_t s = read(0, c, 1);
  
  while(s > 0) {
    if(!IOErrorCheck()) {
      exit(1);
    }
    
      for(int i = 0; i < strlen(from); i++) {
	if(c[0] == from[i]) {
	  c[0] = to[i];
	  break;
	}
      }
   
    write(1,c,sizeof(char)); 
  }
  return 0;
}

