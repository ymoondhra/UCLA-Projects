//NAME: Yash Moondhra                                                           
//Email: ymoondhra@gmail.com                                                    
//ID: 123456789

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>

int nthreads;
int iterations;
bool yield;
char sync;
bool mutex;
bool spin;
int spinVariable;
bool cmp;
long long count;
int write;

pthread_mutex_t lock;

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if (yield)
    sched_yield();
  *pointer = sum;
}

void add_sync(long long value) {
  long long added, anticipated;
  do {
    added = count;
    anticipated = added + value;
    if(yield) {
      sched_yield();
    }
  }
  while(__sync_val_compare_and_swap(&count, added, anticipated) != added);
}

void * addAndSubtract() {
  int c = 0;
  for(; c < iterations; c++) {
    if(mutex) {
      pthread_mutex_lock(&lock);
    }
    else if(spin) {
      while(__sync_lock_test_and_set(&spinVariable, 1));
    }
    
    add(&count, 1);
    
    if(mutex) {
      pthread_mutex_unlock(&lock);
    }
    else if(spin) {
      __sync_lock_release(&spinVariable);
    }
    else if(cmp) {
      add_sync(1);
    }
  }
  for(c = 0; c < iterations; c++) {
    if(mutex) {
      pthread_mutex_lock(&lock);
    }
    else if(spin) {
      while(__sync_lock_test_and_set(&spinVariable, 1));
    }
    
    add(&count, -1);
    
    if(mutex) {
      pthread_mutex_unlock(&lock);
    }
    else if(spin) {
      __sync_lock_release(&spinVariable);
    }
    else if(cmp) {
      add_sync(-1);
    }
  }
  return NULL;
}

void print_data(struct timespec start, struct timespec end) {
  if(!yield && sync=='z') {
    fprintf(stdout, "add-none,");
  }
  else if(!yield && mutex) {
    fprintf(stdout, "add-m,");
  }
  else if (!yield && spin) {
    fprintf(stdout, "add-s,");
  }
  else if(!yield && cmp) {
    fprintf(stdout, "add-c,");
  }
  else if(yield && sync=='z') {
    fprintf(stdout, "add-yield-none,");
  }
  else if(yield && mutex) {
    fprintf(stdout, "add-yield-m,");
  }
  else if(yield && spin) {
    fprintf(stdout, "add-yield-s,");
  }
  else if(yield && cmp) {
    fprintf(stdout, "add-yield-c,");
  }
  
  int operations = 2 * iterations * nthreads;
  int totalTime = (1000000000 * end.tv_sec) - (1000000000 * start.tv_sec) + end.tv_nsec - start.tv_nsec;
  int avgTime = totalTime/operations;
  fprintf(stdout, "%d,%d,%d,%d,%d,%lld\n", nthreads, iterations, operations, totalTime, avgTime, count);
}

int main(int argc, char* argv[]) {
  nthreads = 1;
  iterations = 1;
  yield = false;
  sync = 'z';
  mutex = false;
  spin = false;
  cmp = false;
  count = 0;
  write = 0;
  spinVariable = 0;
  
  static struct option long_options[] = {
      {"threads", required_argument, 0, 't'},
      {"iterations", required_argument, 0, 'i'},
      {"yield", 0, 0, 'y'},
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
    }
    else if(opt_character == 's') {
      sync = *optarg;
      if(sync == 'm') {
	mutex = true;
      }
      else if(sync == 's') {
	spin = true;
      }
      else if(sync == 'c') {
	cmp = true;
      }
    }
    else if(opt_character == -1) {
      break;
    }  
  }

  //Code adapted from my CS 35L project
  pthread_t threads[nthreads]; //the array of actual threads

  int c = 0;
  struct timespec start;
  if(clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
    fprintf(stderr, "Error: could not get time");
    exit(1);
  }

  //call the threads on the function
  for (c = 0; c < nthreads; c++) {
    if(pthread_create(&threads[c], NULL, addAndSubtract, NULL)) {
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
  
  //clock call
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &end);
  
  //write
  print_data(start, end);
  
  return 0;
}
