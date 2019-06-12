//NAME: Yash Moondhra
//EMAIL: ymoondhra@gmail.com
//ID: 105-010-814

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//function for --catch option
void catcher(int sig) {
  fprintf(stderr, "Error: --segfault caused a segmentation fault. Signal code: %d\n", sig);
  exit(4);
}

int main(int argc, char **argv) {
  //segFault is 0 if the --segfault option is not used. 1 if --sefault is used
  int segFault = 0;

  //long_options is passed as a parameter to getopt_long
  static struct option long_options[] = {
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"segfault", no_argument, 0, 's'},
    {"catch", no_argument, 0, 'c'},
    {0,0,0,0}
  };

  //OPTIONS
  //The following while loop
  //  was adapted from http://man7.org/linux/man-pages/man3/getopt.3.html
  //iterates through each option until there are no options left
  while(1) {
    int optindex = 0;
    //int c stores the option character of the current option we are looking at
    int c = getopt_long(argc, argv, "",
		    long_options, &optindex);

    //if there are no options left, go to the reading and writing section
    if(c == -1) {
      break;
    }

    switch(c) {
    case 'i': { //--input=filename
      int openFile = open(optarg, O_RDONLY);
      	if(openFile < 0) {
	  if(errno == EACCES) {
	    //Error: input file exists but no permission to access it
	    fprintf(stderr, "Error: input file %s exists but could not access it. Error code: %s\n", optarg, strerror(errno));
	  }
	  else if(errno == ENOENT) {
	    //Error: input file does not exist
	    fprintf(stderr, "Error: input file %s does not exist. Error code: %s\n", optarg,strerror(errno));
	  }
	  else {
	    //general error
	    fprintf(stderr, "Error: could not open input file %s. Error code: %s\n", optarg,strerror(errno));
	  }
	  exit(2);
	}
	else {
	  close(0);
	  dup(openFile);
	  close(openFile);
	}
        break;
    }
    case 'o': { //--output=filename
	int outputFile = creat(optarg, 0666);
	if(outputFile < 0) {
	  if(errno == EPERM) {
	    //output file permission error
	    fprintf(stderr, "Error: option --output for output file %s could not be permitted. Error code: %s\n", optarg, strerror(errno));
	  }
	  if(errno == EACCES) {
	    //output file access error
	    fprintf(stderr, "Error: output file %s could not be accessed. Error code: %s\n", optarg,strerror(errno));
	  }
	  else {
	    //general error
	    fprintf(stderr, "Error: option --output failed for output file %s. Error code: %s\n", optarg, strerror(errno));
	  }
	  exit(3);
	}
	else {
	  close(1);
	  dup(outputFile);
	  close(outputFile);
	}
        break;
    }
    case 's': { //--segFault
	segFault = 1;
	break;
    }
    case 'c': { //--catch
      signal(SIGSEGV, catcher);
      break;
    }
    default: {
      fprintf(stderr, "Error: unknown option provided. Available options are:\n--input=filename\n--output=filename\n--segfault\n--catch\n");
      exit(1);
    }
    }
  }

  //if the user used option --segFault, cause a segmentation fault
  if(segFault > 0) {
    char* seg = NULL;
    seg[0] = 'a';
  }

  //READING AND WRITING
  char onechar; //stores the value of the currently examined character from stdin
  int readval = read(0, &onechar, sizeof(char)); //reads in one character from stdin
  int writeval; 
  while(readval != 0) { //while we have not reached the end of the file...
    if(readval < 0) {
      fprintf(stderr, "Error: read command failed. Error code: %s\n", strerror(errno));
      exit(-1);
    }
    else {
      writeval = write(1, &onechar, sizeof(char)); //writes one character
      if(writeval < 0) {
	fprintf(stderr, "Error: write command failed. Error code: %s\n", strerror(errno));
	exit(-1);
      }
    }
    readval = read(0, &onechar, sizeof(char));
  }
  
  exit(0);
}
