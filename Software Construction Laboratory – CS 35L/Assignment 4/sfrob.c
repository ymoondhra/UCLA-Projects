#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//takes the XOR of int 42 with char c. This works                                         
//because char c can be converted to int                                                  
char defrob(const char c) {
    return c^42;
}
//compares two strings to see if one is greater or they                                   
//are both equal                                                                          
int frobcmp(char const * a, char const * b) {
    for(;; a++, b++) {
        while(*a == '\0')
        {
            a++;
        }
        while(*b == '\0')
        {
            b++;
        }
        if(*a == ' ' && *b == ' ') {
            return 0; }
        else if((*a == ' ') || (defrob(*a) < defrob(*b)) ) {
            return -1;
        }
        else if((*b == ' ')|| (defrob(*a) > defrob(*b)) ) {
            return 1;
        }
    }
}
//comparing two c-strings using frobcmp                                                   
int compare(const void* x, const void* y) {
    char const* first = *(const char **) x;
    char const* second = *(const char **) y;
    return frobcmp(first, second);
}
//checks for io error                                                                     
int IOErrorCheck(void) {
    if(ferror(stdin)) {
        fprintf(stderr, "There is an Input / Output Error. Exiting now. \n");
        return 0;
    }
    return 1;
}

int main(void) {
  char* cs;
  char** cstringarray;

  cs=(char*)malloc(sizeof(char));
  cstringarray = malloc(sizeof(char*));
  if(cs == NULL) {
    fprintf(stderr, "Memory Allocation Failed\n");
    exit(1);
  }

  int x = 0; //for length of cs
  int y = 0; //for length of cstringarray

  cs[0] = getchar(); //random value
  bool newword = true;
  bool lastword = false;
  
  while(cs[x] != EOF && IOErrorCheck()) {    
    if(!IOErrorCheck()) {
	free(cs);
        for (int i = 0; i < y; i++)
            free(cstringarray[i]);
        free(cstringarray);
        exit(1);
      }
    
    if(cs[x] == ' ') {
      if(newword) {
       	newword = false;
	
	char** t = realloc(cstringarray, y*sizeof(char*) + sizeof(char*));
	
        if(t != NULL) {
	  cstringarray = t;
	  cstringarray[y] = cs;
	  y++;
	    x = -1;
	    cs = (char*)malloc(sizeof(char));
        }
        else {
          free (cs);
          for (int i = 0; i < y; i++)
              free(cstringarray[i]);
          free(cstringarray);
          fprintf(stderr, "Memory Allocation Failed\n");
          exit(1);
	}
      }
      else {
	newword = false;
	x = -1;
      }
    }
    else {
      newword = true;
    }
    if(lastword) {
      break;
    }
    char next = getchar();
    if(next == EOF) {
      lastword = true;
      next = ' ';
    }
    x++;
    char* tr = realloc(cs, x*sizeof(char) + sizeof(char));
    if(tr != NULL) {
      cs = tr;
      cs[x] = next;
    }
    else {
      free(cs);
      for(int q = 0; q < y; q++) {
        free(cstringarray[q]);
      }
      free(cstringarray);
      fprintf(stderr, "Memory Allocation Failed\n");
      exit(1);
    }
  }
  qsort(cstringarray, y, sizeof(char*), compare);

  //send to stdout
  int a;
  int b;
  for(a = 0; a < y; a++) {
      for(b = 0; cstringarray[a][b] != ' '; b++) {
          printf("%c", cstringarray[a][b]);
      }
      printf(" ");
  }

  //free up the memory                                                        
  for(a = 0; a < y; a++) {
      free(cstringarray[a]);
  }
  free(cstringarray);
  free(cs);
  return 0;
}
