#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
  char* from = argv[1];
  char* to = argv[2];


  if(strlen(from) != strlen(to)) {
    fprintf(stderr, "Error: the decrypted string did not match the encrypted string");
    exit(1);
  }

  if(argc > 3) {
    fprintf(stderr, "Error: too many args provided. Quitting...\n");
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

  int c = getchar();
  while(c != EOF) {
    //if(!IOErrorCheck()) {
    //   exit(1);
    //}
      for(int i = 0; i < strlen(from); i++) {
	if(c == from[i]) {
	  c = to[i];
	   break;
	}	
      }
    putchar(c);
    c = getchar();
  }
  return 0;
}

