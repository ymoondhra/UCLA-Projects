/*
NAME: Yash Moondhra
EMAIL: ymoondhra@gmail.com
UID: 123456789
 */
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/wait.h>

struct termios pastTerm;
int pipe1[2];
int pipe2[2];
pid_t cpid;
int s;

void reset(void) {
  //reset terminal attributes to original
  if(tcsetattr(STDIN_FILENO, TCSANOW, &pastTerm) < 0) {
    fprintf(stderr, "Error: could not set terminal attributes");
    exit(1);
  }
  int status;
  if(s == 's') {
    //wait for child (shell) task to complete
      if(waitpid(cpid, &status, 0) < 0) {
	       fprintf(stderr, "Error: child process not completed");
	       exit(1);
      }
      //output the appropriate error code
      fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
      exit(0);
  }
}

void noShellOption() {
  char c[1000];
  int numRead = read(STDIN_FILENO, &c, 1000*sizeof(char));
  while(numRead != 0) {
    int j;
    for(j=0; j < numRead; j++) {
       if(c[j] == '\0' || c[j] == '\004') {
       exit(0);
       break;
    }
    else if(c[j] == '\r' || c[j] == '\n') {
       char d = '\r';
       write(1, &d, sizeof(char));
       d = '\n';
       write(1, &d, sizeof(char));
    }
    else {
       write(1, &c[j], sizeof(char));
      }
    }
    memset(c, 0, 1000*sizeof(char));
    numRead = read(STDIN_FILENO, &c, 1000*sizeof(char));
  }
}

void shellOption() {
  //create two new unidirectional pipes
  if(pipe(pipe1) == -1 || pipe(pipe2) == -1) {
      fprintf(stderr, "Error: could not create pipe");
      exit(1);
  }
    //create new process
    cpid = fork();
    if(cpid == -1) {
      fprintf(stderr, "Error: could not fork");
      exit(1);
    }

    if(cpid == 0) { //child
      close(pipe1[1]);
      close(pipe2[0]);
      close(0);
      dup(pipe1[0]);
      close(1);
      dup(pipe2[1]);
      close(2);
      dup(pipe2[1]);
      close(pipe1[0]);
      close(pipe2[1]);
      execl("/bin/bash", "shell", (char*) NULL);
      //if execl succeeds, it will never reach the following line
      fprintf(stderr, "Error: failed to create shell");
      exit(1);
    }
    else { //parent
      close(pipe1[0]);
      close(pipe2[1]);
      
      struct pollfd polls[2];
      polls[0].fd = 0;
      polls[1].fd = pipe2[0];
      polls[1].events = POLLIN | POLLHUP | POLLERR;
      polls[0].events = POLLIN | POLLHUP | POLLERR;

      char parentBuffer[1000];
      while(1) {
	       memset(parentBuffer, 0, 1000*sizeof(char));
	       int poller = poll(polls, 2, -1);
	       if(poller > 0) {
	         if(polls[0].revents & POLLIN) {
	           int red = read(polls[0].fd, &parentBuffer, 1000*sizeof(char));
	           if(red < 0) {
	             fprintf(stderr, "Error: read failed from standard input");
	             exit(1);
	           }
	           int j;
	           for(j=0; j < red; j++) {
  	           if(parentBuffer[j] == '\04') {
  		          close(pipe1[1]);
  	           }
        	      else if(parentBuffer[j] == '\03') {
        		      kill(cpid, SIGINT);
        	      }
        	      else if(parentBuffer[j] == '\r' || parentBuffer[j] == '\n') {
              		char d = '\r';
              		write(1, &d, sizeof(char));
              		d = '\n';
              		int writer = write(pipe1[1], &d, sizeof(char));
              		write(1, &d, sizeof(char));
              		if(writer < 0) {
              		  fprintf(stderr, "Error: could not write to shell");
              		  exit(1);
              		}
        	      }
        	      else {
        		      write(pipe1[1], &parentBuffer[j], sizeof(char));
        		      write(1, &parentBuffer[j], sizeof(char));
        	      }
	           }
	         }
      	  else if(polls[0].revents & (POLLHUP | POLLERR)) {
      	    exit(1);
      	  }
          if(polls[1].revents & POLLIN) {
            int red = read(polls[1].fd, &parentBuffer, 1000*sizeof(char));
            if(red < 0) {
                fprintf(stderr, "Error: read failed from shell");
                exit(1);
      	    }
      	    int j;
      	    for(j=0; j < red; j++) {
      	      if(parentBuffer[j] == '\n') {
            		char d = '\r';
            		write(1, &d, sizeof(char));
            		d = '\n';
            		write(1, &d, sizeof(char));
      	      }
      	      else {
      		      write(1, &parentBuffer[j], sizeof(char));
      	      }
      	    }
      	  }
          else if(polls[1].revents & (POLLHUP | POLLERR)) {
             exit(1);
          }
	     }
    	else if(poller == -1) {
    	  fprintf(stderr, "Error: could not poll");
    	  exit(1);
    	}
    }    
  }
}

int main(int argc, char** argv) {
  //store old terminal settings and set a function to run at exit
  if(tcgetattr(STDIN_FILENO, &pastTerm) < 0) {
    fprintf(stderr, "Error: could not get terminal attributes");
    exit(1);
  }
  atexit(reset);

  //"--shell" option setup
  static struct option long_options[] = {
    {"shell", no_argument, 0, 's'},
  	{0,0,0,0},
  };  
  int optindex = 0;
  s = getopt_long(argc,argv, "", long_options, &optindex);

  //set up new terminal settings
  struct termios newTerm;
  tcgetattr(STDIN_FILENO, &newTerm);
  newTerm.c_iflag = ISTRIP;
  newTerm.c_oflag = 0;
  newTerm.c_lflag = 0;
  newTerm.c_cc[VMIN] = 1;
  newTerm.c_cc[VTIME] = 0;
  if(tcsetattr(STDIN_FILENO, TCSANOW, &newTerm) < 0) {
    fprintf(stderr, "Error: could not set terminal attributes");
    exit(1);
  }
  //if user passed in --shell option, call the appropriate function
  if(s == 's') {
    shellOption();
  }
  else if(s == '?') {
    fprintf(stderr, "Error: bad argument. Use --shell instead");
    exit(1);
  }
  else {
    noShellOption();
  }
  
  return 0;
}
