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
int * spin; //spin lock
bool m; //mutex
SortedList_t** list;
SortedListElement_t* listElements;
pthread_mutex_t * lock;

long long totalWaitTime;
long long waitTime;
long long * elapsed;

int lists;

//Hash Function Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void handleSegFault(int signalNumber) {
  fprintf(stderr, "Error %d: %s", signalNumber, strerror(errno));
  exit(2);
}

void insert_to_list(int f, int end)  {
  int threadNum = f;
  for(; f < end; f += nthreads) {
    const char * elementKey = listElements[f].key;
    unsigned long hashVal = hash(elementKey)%lists;
    int d = hashVal;
    if(s) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      while(__sync_lock_test_and_set(&spin[d], 1));
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[threadNum] += threadWaitTime;
    }
    else if(m) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      pthread_mutex_lock(&lock[d]);
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[threadNum] += threadWaitTime;
    }
    SortedList_insert(list[d], &listElements[f]);
    if(s) {
      __sync_lock_release(&spin[d]);
    }
    else if(m) {
      pthread_mutex_unlock(&lock[d]);
    }
    
  }
}

void * listFunction(void* threadNum) {
  int currThread = *((int *) threadNum);
  int f = currThread;
  int end = nthreads * iterations;
  insert_to_list(f, end);

  //LENGTH
  int length = 0;
  int u = 0;
  for(u = 0; u < lists; u++) {
    if(s) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      while(__sync_lock_test_and_set(&spin[u], 1));
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[currThread] += threadWaitTime;
      length += SortedList_length(list[u]);
      __sync_lock_release(&spin[u]);
    }
    else if(m) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      pthread_mutex_lock(&lock[u]);
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[currThread] += threadWaitTime;
      length += SortedList_length(list[u]);
      pthread_mutex_unlock(&lock[u]);
    }
    else {
      length += SortedList_length(list[u]);
    }
  }
  if(length == -1) {
    fprintf(stderr, "Error: improper list length from thread number: %d", currThread);
    exit(2);
  }

  
  for(f = currThread; f < end; f+= nthreads) {
    const char * elementKey = listElements[f].key;
    unsigned long hashVal = hash(elementKey)%lists;
    int d = hashVal;
    SortedListElement_t* t;
    if(s) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      while(__sync_lock_test_and_set(&spin[d], 1));
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[currThread] += threadWaitTime;
      t = SortedList_lookup(list[d], listElements[f].key);
      if(SortedList_delete(t)) {
	fprintf(stderr, "Error: could not delete element in thread %d", currThread);
	exit(2);
      }
      __sync_lock_release(&spin[d]);
    }
    else if(m) {
      struct timespec startTimer;
      if(clock_gettime(CLOCK_MONOTONIC, &startTimer) == -1) {
	fprintf(stderr, "Error: could not get time");
	exit(1);
      }
      pthread_mutex_lock(&lock[d]);
      struct timespec endTimer;
      clock_gettime(CLOCK_MONOTONIC, &endTimer);
      long long threadWaitTime = (1000000000 * endTimer.tv_sec) - (1000000000 * startTimer.tv_sec) + endTimer.tv_nsec - startTimer.tv_nsec;
      elapsed[currThread] += threadWaitTime;
      t = SortedList_lookup(list[d], elementKey);
      if(SortedList_delete(t)) {
	fprintf(stderr, "Error: could not delete element in thread %d", currThread);
	exit(2);
      }
      pthread_mutex_unlock(&lock[d]);
    }
    else {
      t = SortedList_lookup(list[d], elementKey);
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
  
  return NULL;
}

void print_data(struct timespec start, struct timespec end) {
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
  
  if(m || s) {
    int q = 0;
    while(q < nthreads) {
      totalWaitTime += elapsed[q];
      q++;
    }
  }
  
  int operations = 3*nthreads*iterations;
  long long totalTime = (1000000000 * end.tv_sec) - (1000000000 * start.tv_sec) + end.tv_nsec - start.tv_nsec;
  long long avgTime = totalTime/operations;

  waitTime = totalWaitTime/operations;
  
  fprintf(stdout, "list-%s-%s,%d,%d,%d,%d,%lld,%lld,%lld\n", yieldOptions, syncOption, nthreads, iterations, lists, operations, totalTime, avgTime, waitTime);
}

int main(int argc, char* argv[]) {
  nthreads = 1;
  iterations = 1;
  yield = false;
  i = false;
  d = false;
  l = false;
  s = false; spin = NULL;
  m = false;
  lock = NULL;
  
  totalWaitTime = 0;
  waitTime = 0;
  lists = 1;
  
  static struct option long_options[] = {
      {"threads", required_argument, 0, 't'},
      {"iterations", required_argument, 0, 'i'},
      {"yield", required_argument, 0, 'y'},
      {"sync", required_argument, 0, 's'},
      {"lists", required_argument, 0, 'l'},
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
    else if(opt_character == 'l') {
      lists = atoi(optarg);
      if(lists < 1) {
	fprintf(stderr, "Error: --lists argument must be greater than 0");
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
      if(*optarg == 's') {
	s = true;
	elapsed = (long long *) malloc(nthreads * sizeof(long));
	int q = 0;
	while(q < nthreads) {
	  elapsed[q] = 0;
	  q++;
	}
      }
      else if(*optarg == 'm') {
	m = true;
	elapsed = (long long *) malloc(nthreads * sizeof(long));
	int q = 0;
	while(q < nthreads) {
	  elapsed[q] = 0;
	  q++;
	}
      }
      else {
	fprintf(stderr, "Error: unrecognized argument for --sync option. Use either s or m");
	exit(1);
      }
    }
    else if(opt_character == -1) {
      break;
    }
  }

  int u = 0;
  if(m) {
    lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*lists);
    //for(u = 0; u < lists; u++) {
    //  lock[u] = 0;
    //}
    memset(lock, 0, lists * sizeof(int));
  }
  else if(s) {
    spin = (int *) malloc(sizeof(int)*lists);
    //for(u = 0; u < lists; u++) {
    //  spin[u] = 0;
    // }
    memset(spin, 0, lists * sizeof(int));
  }


  //Initializing the list
  list = (SortedList_t**) malloc(sizeof(SortedList_t*) * lists);
  for(u = 0; u < lists; u++) {
    list[u] = (SortedList_t*) malloc(sizeof(SortedList_t));
    list[u]->next = list[u];
    list[u]->prev = list[u];
    list[u]->key = NULL;
  }

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
   if (m && pthread_mutex_init(&lock[0], NULL) != 0) {
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
  if (m && pthread_mutex_destroy(&lock[0]) != 0) {
    fprintf(stderr, "Error: could not destroy mutex lock");
    exit(1);
  }

  //clock call
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);

  // fprintf(stderr, "after second clock call");

  //ensure that the length of the list is 0
  if(SortedList_length(list[0]) > 0) {
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
  for(u = 0; u < lists; u++) {
    free(list[u]);
  }
  free(list);
  if(m || s) {
    free(elapsed);
  }
  
  return 0;
}
