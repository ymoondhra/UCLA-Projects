//NAME: Yash Moondhra                                                           
//Email: ymoondhra@gmail.com                                                    
//ID: 123456789

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "SortedList.h"
#include <signal.h>
#include <errno.h>

int nthreads;
int iterations;
int opt_yield = 0;
bool yield;
bool i; //insert
bool d; //delete
bool l; //lookup
bool s; //spin
int spin; //spin lock
bool m; //mutex
SortedList_t* list;
SortedListElement_t* listElements;

pthread_mutex_t lock;

void handleSegFault(int signalNumber) {
  fprintf(stderr, "Error %d: %s", signalNumber, strerror(errno));
  exit(2);
}

void insert_to_list(int f, int end)  {
  for(; f < end; f += nthreads) {
    // fprintf(stderr, "inside for loop of insert_to_list. f: %d\n", f);
    if(s) {
      while(__sync_lock_test_and_set(&spin, 1));
    }
    else if(m) {
      pthread_mutex_lock(&lock);
    }
    SortedList_insert(list, &listElements[f]);
    if(s) {
      __sync_lock_release(&spin);
    }
    else if(m) {
      pthread_mutex_unlock(&lock);
    }
    // fprintf(stderr, "after insert of for loop of insert_to_list. f: %d\n", f);
    
  }
}

void * listFunction(void* threadNum) {
  int currThread = *((int *) threadNum);
  int f = currThread;
  int end = nthreads * iterations;
   // fprintf(stderr, "inside list function. Thread num: %d\n", currThread);
  insert_to_list(f, end);
   // fprintf(stderr, "after insert_to_list");

  // fprintf(stderr, "Getting length");
  //LENGTH
  int length;
  if(s) {
    while(__sync_lock_test_and_set(&spin, 1));
    length = SortedList_length(list);
    __sync_lock_release(&spin);
  }
  else if(m) {
    pthread_mutex_lock(&lock);
    length = SortedList_length(list);
    pthread_mutex_unlock(&lock);
  }
  else {
    // fprintf(stderr, "about to get length\n");
    length = SortedList_length(list);
  }
  if(length == -1) {
    fprintf(stderr, "Error: improper list length from thread number: %d", currThread);
    exit(2);
  }
  
  for(f = currThread; f < end; f+= nthreads) {
    SortedListElement_t* t;
    if(s) {
      while (__sync_lock_test_and_set(&spin, 1));
      t = SortedList_lookup(list, listElements[f].key);
      if(SortedList_delete(t)) {
	fprintf(stderr, "Error: could not delete element in thread %d", currThread);
	exit(2);
      }
      __sync_lock_release(&spin);
    }
    else if(m) {
      pthread_mutex_lock(&lock);
      t = SortedList_lookup(list, listElements[f].key);
      if(SortedList_delete(t)) {
	fprintf(stderr, "Error: could not delete element in thread %d", currThread);
	exit(2);
      }
      pthread_mutex_unlock(&lock);
    }
    else {
      t = SortedList_lookup(list, listElements[f].key);
      if(SortedList_delete(t)) {
	fprintf(stderr, "Error: could not delete element in thread %d", currThread);
	exit(2);
      }
    }
    if(t == NULL) {
      fprintf(stderr, "Error: could not find element in thread %d", currThread);
      exit(2);
    }
  }
   // fprintf(stderr, "after lookup");
  
  return NULL;
}

void print_data(struct timespec start, struct timespec end) {
  // fprintf(stderr, "bout to printtt");
  char yieldOptions[10] = "";
  char none[10] = "none";
  if(i) {
    yieldOptions[0] = 'i';
    if(d) {
      yieldOptions[1] = 'd';
      if(l)
	yieldOptions[2] = 'l';
    }
    else {
      if(l) {
	yieldOptions[1] = 'l';
      }
    }
    //fprintf(stderr, "%s\n", yieldOptions);
  }
  else {
    if(d) {
      yieldOptions[0] = 'd';
    }
    else {
      if(l) {
	yieldOptions[0] = 'l';
      }
    }
    //fprintf(stderr, "%s\n", yieldOptions);
  }
  if(!i && !d && !l) {
    strcpy(yieldOptions, none);
  }
  
  char syncOption[10] = "";
  if(s)
    syncOption[0] = 's';
  else if(m)
    syncOption[0] = 'm';
  else {
    strcpy(syncOption, none);
  }
  
  int operations = 3*nthreads*iterations;
  int totalTime = (1000000000 * end.tv_sec) - (1000000000 * start.tv_sec) + end.tv_nsec - start.tv_nsec;
  int avgTime = totalTime/operations;
  fprintf(stdout, "list-%s-%s,%d,%d,1,%d,%d,%d\n", yieldOptions, syncOption, nthreads, iterations, operations, totalTime, avgTime);
}

int main(int argc, char* argv[]) {
  nthreads = 1;
  iterations = 1;
  yield = false;
  i = false;
  d = false;
  l = false;
  s = false; spin = 0;
  m = false;
  
  static struct option long_options[] = {
      {"threads", required_argument, 0, 't'},
      {"iterations", required_argument, 0, 'i'},
      {"yield", required_argument, 0, 'y'},
      {"sync", required_argument, 0, 's'},
      {0,0,0,0},
  };
  
  int optindex = 0;
  while(1) {
    int opt_character = getopt_long(argc, argv, "", long_options, &optindex);
    if(opt_character == '?') {
      fprintf(stderr, "Error: unrecognized argument. Use --threads=# and/or --iterations=#");
      exit(1);
    }
    if(opt_character == 't') {
      nthreads = atoi(optarg);
      if(nthreads < 1) {
	fprintf(stderr, "Error: --threads value must be greater than or equal to 1.");
	exit(1);
      }
    }
    else if(opt_character == 'i') {
      iterations = atoi(optarg);
      if(iterations < 1) {
	fprintf(stderr, "Error: --iterations value must be greater than or equal to 1.");
	exit(1);
      }
    }
    else if(opt_character == 'y') {
      yield = true;
      char* yieldOptions = optarg;
      unsigned int u = 0;
      for(; u < strlen(yieldOptions); u++) {
	switch(yieldOptions[u]) {
	case 'i': i = true; opt_yield |= INSERT_YIELD;  break;
	case 'l': l = true; opt_yield |= DELETE_YIELD; break;
	case 'd': d = true; opt_yield |= LOOKUP_YIELD; break;
	default: fprintf(stderr, "Error: could not recognize argument for --yield option. Use: i, d, and/or l."); exit(1); 
	}
      }
    }
    else if(opt_character == 's') {
      if(*optarg == 's')
	s = true;
      else if(*optarg == 'm')
	m = true;
      else {
	fprintf(stderr, "Error: unrecognized argument for --sync option. Use either s or m");
	exit(1);
      }
    }
    else if(opt_character == -1) {
      break;
    }
  }


  //Initializing the list
  list = malloc(sizeof(SortedList_t));
  list->next = list;
  list->prev = list;
  list->key = NULL;
  //Initializing the list elements
  char* CHARSET = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int numElements = nthreads*iterations;
  listElements = (SortedListElement_t*) malloc(numElements* sizeof(SortedListElement_t));
  srand(time(NULL));
  int elementCounter = 0;
  for(; elementCounter < numElements; elementCounter++) {
    char* key = (char*) malloc(3*sizeof(char));
    key[0] = CHARSET[rand()%62];
    key[1] = CHARSET[rand()%62];
    key[2] = '\0';
    listElements[elementCounter].key = key;
  }

  //Code adapted from my CS 35L project
  pthread_t threads[nthreads]; //the array of actual threads
  int threadID[nthreads]; //the array of thread IDs
  
  int i = 0;
  //setting the thread ids to their respective values
  for (i = 0; i < nthreads; i++) {
    threadID[i] = i;
  }

  //segfault handler
   if(signal(SIGSEGV, handleSegFault) == SIG_ERR) {
    fprintf(stderr, "Error: could not set up signal handler");
    exit(1);
   }

   //init mutex
   if (m && pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error: could not initialize mutex lock");
      } 

  struct timespec start;
  if(clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    fprintf(stderr, "Error: could not get time");
    exit(1);
  }

  int c = 0;
  //call the threads on the function
  for (c = 0; c < nthreads; c++) {
    if(pthread_create(&threads[c], NULL, listFunction, &threadID[c])) {
      fprintf(stderr, "Error: Failed to generate new thread");
      exit(1);
    }
  }
  
  //join the threads when they are completed
  for (c = 0; c < nthreads; c++) {
    if(pthread_join(threads[c], NULL)) {
      fprintf(stderr, "Error: Failed to join threads");
      exit(1);
    }
  }

      //Destroy mutex
  if (m && pthread_mutex_destroy(&lock) != 0) {
    fprintf(stderr, "Error: could not destroy mutex lock");
    exit(1);
  }

  //clock call
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);

  // fprintf(stderr, "after second clock call");

  //ensure that the length of the list is 0
  if(SortedList_length(list) > 0) {
    fprintf(stderr, "List not cleaned up properly");
  }

  // fprintf(stderr, "bout to print data]\n");

  print_data(start,end);

  //free memory
  int k = 0;
  // fprintf(stderr, "bout to clean keys\n");
  for(; k < numElements; k++) {
    free((void*) listElements[k].key);
  }
  free(listElements);
  free(list);
  
  return 0;
}
