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
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <poll.h>
#include <mraa.h>
#include <mraa/aio.h>

int period;
char scale;
bool logOption;
char* logFile;
int log_fd; //file descriptor for log file

bool running;


char buffer[1000];

//MRAA
mraa_aio_context sensor;
mraa_gpio_context button;

void closeSensors() {
  if(mraa_aio_close(sensor) != MRAA_SUCCESS) {
    fprintf(stderr, "Error: could not exit AIO properly\n");
    exit(1);
  }
  mraa_gpio_close(button);
  exit(0);
}

void pressButton(struct timeval now) {
  if(gettimeofday(&now, NULL) == -1) {
    fprintf(stderr, "Error: could not get time of day\n");
    exit(1);
  }

  struct tm* info = localtime(&now.tv_sec);

  sprintf(buffer,"%.2d:%.2d:%.2d SHUTDOWN\n", info->tm_hour,info->tm_min, info->tm_sec);
  fprintf(stdout, "%s", buffer);
  if(logOption) {
    dprintf(log_fd, "%s", buffer);
  }
  exit(0);
}

void handleInput(const char* copiedBuffer, struct timeval now) {  
  if(strncmp(copiedBuffer, "SCALE=F", 7) == 0) {
    scale = 'F';
  }
  else if(strncmp(copiedBuffer, "SCALE=C", 7) == 0) {
    scale = 'C';
  }
  else if(strncmp(copiedBuffer, "PERIOD=", 7) == 0) {
    const char* bufferAfterPERIOD = copiedBuffer + 7;
    period = atoi(bufferAfterPERIOD);
    if(period <= 0) {
      fprintf(stderr, "Error: period must be greater than 0: %s . . . %d is period\n", bufferAfterPERIOD, period);
      exit(1);
    }
  }
  else if(strncmp(copiedBuffer, "STOP", 4) == 0) { 
    running = false;
  }
  else if(strncmp(copiedBuffer, "START", 5) == 0) {
    running = true;
  }
  else if(strncmp(copiedBuffer, "LOG", 3) == 0 && logOption) {
    dprintf(log_fd, "%s\n", copiedBuffer);
    return;
  }

  if(logOption) {
    dprintf(log_fd, "%s\n", copiedBuffer);
  }
  if(strncmp(copiedBuffer, "OFF", 3) == 0) {
    pressButton(now);
 	}
  return;
}

double calculateTemp(int mraaReading) { //converts mraa reading into a temperature
  int beta = 4275;
  double t = 100000.0 * (1023.0 / ((double) mraaReading) - 1.0);
  double temperature = 1.0 / (log(t/100000.0)/beta + (1.0/298.15)) - 273.15;
  if(scale == 'F') {
    temperature = (9 * temperature/5) + 32;
  }
  return temperature;
}

void getTemperature() {
  memset(buffer, 0, 1000);
  struct timeval now, next;
  next.tv_sec = 0;
  
  //this code was adapted from Lab 1B
  struct pollfd polls[1];
  polls[0].fd = 0; //STDIN
  polls[0].events = POLLIN | POLLHUP | POLLERR;

  while(1) {
    if(gettimeofday(&now, 0) == -1) {
      fprintf(stderr, "Error: could not get time\n");
      exit(1);
    }
    double temperature = calculateTemp(mraa_aio_read(sensor));

    if(now.tv_sec >= next.tv_sec && running) {
      //get data
      struct tm* info = localtime(&now.tv_sec);
      if(info == NULL) {
	fprintf(stderr, "Error: could not get time\n");
	exit(1);
      }
      //print data
      char data[1000];
      sprintf(data, "%02d:%02d:%02d %.1f", info->tm_hour, info->tm_min, info->tm_sec, temperature);
      fprintf(stdout, "%s\n", data);
      if(logOption) {
	       dprintf(log_fd, "%s\n", data);
      }
      
      next = now;
      next.tv_sec += period;
    }
    
    int poller = poll(polls, 1, 0);
    if(poller < 0) {
      fprintf(stderr, "Error: could not poll\n");
      exit(1);
    }
    else {
      // if(polls[0].revents & (POLLHUP | POLLERR)) {
	     //   exit(1);
      // }
      if(polls[0].revents & POLLIN) {
    	int numRead = read(STDIN_FILENO, buffer, 1000);
    	if(numRead == -1) {
    	  fprintf(stderr, "Error: could not read from standard input\n");
    	  
    	  exit(1);
	   }
    	int x = 0;
    	int copyx = 0;
    	char copiedBuffer[1000];
    	memset(copiedBuffer, 0, 1000);
    	for(; x < numRead && copyx < 1000; x++) {
    	  if(buffer[x] == '\n') {
    	    buffer[x]='\0';
    	    handleInput((char*) &copiedBuffer, now);
    	    copyx = 0;
    	    memset(copiedBuffer, 0, 1000); 
	      }
    	  else {
    	    copiedBuffer[copyx] = buffer[x];
    	    copyx++;
    	  }
	     }
      }
    }
    if(mraa_gpio_read(button)) {
      pressButton(now);
    }   
  }
  dprintf(log_fd, "ABOUT TO EXIT\nLAST COMMAND WAS %s\n", buffer);
}

int main(int argc, char* argv[]) {
  //initialize global variables
  scale = 'F';
  period = 1;
  logOption = false;
  running = true;

  //set up option input retrieval
  static struct option long_options[] = {
    {"period", required_argument, 0, 'p'},
    {"scale", required_argument, 0, 's'},
    {"log", required_argument, 0, 'l'},
    {0,0,0,0},
  };

  //get option inputs 
  int optindex = 0;
  while(1) {
    int opt_character = getopt_long(argc, argv, "", long_options, &optindex);
    if(opt_character == '?') {
      fprintf(stderr, "Error: unrecognized argument. Use --threads=# and/or --iterations=#\n");
      exit(1);
    }
    if(opt_character == 'p') {
      period = atoi(optarg);
      if(period <= 0) {
	fprintf(stderr, "Error: --threads value must be greater than or equal to 1\n");
	exit(1);
      }
    }
    else if(opt_character == 's') {
      scale = optarg[0];
      if(scale != 'C' && scale != 'F') {
      	fprintf(stderr, "Error: unrecognized argument for --scale option. Use C or F\n");
      	exit(1);
      }
    } 
    else if(opt_character == 'l') {
      logOption = true;
      logFile = optarg;
    }
    else if(opt_character == -1) {
      break;
    }
  }

  if(logOption) {
    log_fd = creat(logFile, 0666);
    if(log_fd == -1) {
      fprintf(stderr, "Error: could not open the log file provided as an option\n");
      exit(1);
    }
  }

  //initialize
  sensor = mraa_aio_init(1);
  if(sensor == NULL) {
    fprintf(stderr, "Error: Could not initialize AIO\n");
    exit(1);
  }
  
  button = mraa_gpio_init(62);
  if(mraa_gpio_dir(button, MRAA_GPIO_IN)) {
    fprintf(stderr, "Error: could not link button to handler");
    exit(1);
  }

  atexit(closeSensors);
  getTemperature();

  return 0;
}
