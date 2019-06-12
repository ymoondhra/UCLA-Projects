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
#include <strings.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <poll.h>
#include <mraa.h>
#include <mraa/aio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int period;
char scale;
bool logOption;
char* logFile;
bool hostOption;
char* hostName;
int log_fd; //file descriptor for log file
int id;
int port;
bool running;

char buffer[1000];
//char printMessage[1000] = "Running while loop";

//MRAA
mraa_aio_context sensor;

//Client and Server
struct hostent* host;
struct sockaddr_in server;
int socket_fd;

void closeSensors() {
  if(mraa_aio_close(sensor) != MRAA_SUCCESS) {
    fprintf(stderr, "Error: could not exit AIO properly");
    exit(1);
  }
  exit(0);
}

void pressButton(struct timeval now) {
  if(gettimeofday(&now, NULL) == -1) {
	fprintf(stderr, "Error: could not get time of day\n");
	exit(1);
  }

  struct tm* info = localtime(&now.tv_sec);
  sprintf(buffer,"%.2d:%.2d:%.2d SHUTDOWN\n", info->tm_hour,info->tm_min, info->tm_sec);
  if(write(STDOUT_FILENO, buffer, strlen(buffer)) == -1) {
    fprintf(stderr, "Error: could not write SHUTDOWN to server");
    exit(1);
  }
  if(logOption) {
    
    if(write(log_fd, buffer, strlen(buffer)) == -1) {
      fprintf(stderr, "Error: could not write SHUTDOWN to log file");
      exit(1);
    }
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
        write(log_fd, copiedBuffer, strlen(copiedBuffer));
        return;
    }
    
    if(logOption) {
        write(log_fd, copiedBuffer, strlen(copiedBuffer));
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
            sprintf(data, "%02d:%02d:%02d %.1f\n", info->tm_hour, info->tm_min, info->tm_sec, temperature);
            write(STDOUT_FILENO, data, strlen(data));
            if(logOption) {
                write(log_fd, data, strlen(data));
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
    }
}

void clientServer() {
  //create socket. Code adapted from Lab 1B
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) {
    fprintf(stderr, "Error: could not create a socket\n");
    exit(1);
  }
  host = gethostbyname(hostName);
  if(host == NULL) {
    fprintf(stderr, "Error: could not get host name\n");
    exit(1);
  }

  server.sin_family = AF_INET;
  bcopy((char *)(host -> h_addr), (char *)&server.sin_addr.s_addr, host->h_length);
  server.sin_port = htons(port);
  if(connect(socket_fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
    fprintf(stderr, "Error: could not connect to server. %s\n", strerror(errno));
    exit(1);
  }
  
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  dup(socket_fd);
  dup(socket_fd);
  close(socket_fd);

  char stringID[15];
  sprintf(stringID, "ID=%d\n", id);
  int written = write(STDOUT_FILENO, stringID, strlen(stringID));
  if(written == -1) {
    fprintf(stderr, "Error: could not write ID to server\n");
    exit(2);
  }
  written = write(log_fd, stringID, strlen(stringID));
  if(written == -1) {
    fprintf(stderr, "Error: could not write ID to log\n");
    exit(2);
  }
}

int main(int argc, char* argv[]) {
  //initialize global variables
  scale = 'F';
  period = 1;
  id = 0;
  port = 0;
  hostName = "";
  logOption = false;
  hostOption = false;
  running = true;
  memset(&server, 0, sizeof(server));
  //set up option input retrieval
  static struct option long_options[] = {
      {"period", required_argument, 0, 'p'},
      {"scale", required_argument, 0, 's'},
      {"log", required_argument, 0, 'l'},
      {"id", required_argument, 0, 'i'},
      {"host", required_argument, 0, 'h'},
      {0,0,0,0},
  };

  //get option inputs 
  int optindex = 0;
  while(1) {
    
    int opt_character = getopt_long(argc, argv, "", long_options, &optindex);
    if(opt_character == '?') {
      fprintf(stderr, "Error: unrecognized argument. Use --threads=# and/or --iterations=#");
      exit(1);
    }
    if(opt_character == 'p') {
      period = atoi(optarg);
      if(period <= 0) {
	fprintf(stderr, "Error: --threads value must be greater than or equal to 1.");
	exit(1);
      }
    }
    else if(opt_character == 's') {
      scale = optarg[0];
      if(scale != 'C' || scale != 'F') {
	fprintf(stderr, "Error: unrecognized argument for --scale option. Use C or F\n");
	exit(1);
      }
    } 
    else if(opt_character == 'l') {
      logOption = true;
      logFile = optarg;
    }
    else if(opt_character == 'i') {
      id = atoi(optarg);
    }
    else if(opt_character == 'h') {
      hostOption = true;
      hostName = optarg;
    }
    else if(opt_character == -1) {
      break;
    }
  }

  if(!logOption || !hostOption) {
    fprintf(stderr, "Error: must provide a log file and host\n");
    exit(1);
  }
  else {
    //create log file
    log_fd = creat(logFile, 0666);
    if(log_fd == -1) {
      fprintf(stderr, "Error: could not open the log file provided as an option\n");
      exit(1);
    }

    //get port number
    port = atoi(argv[argc - 1]);
    if(port == 0) {
      fprintf(stderr, "Error: must provide a port number as the last argument\n");
      exit(1);
    }
  }

  //check that ID is 9 digits
  int tempId = id;
  int i = 0;
  while(i < 8) {
    tempId = tempId/10;
    i++;
  }
  if(tempId > 9 || tempId < 1) {
    fprintf(stderr, "Error: you must provide an id that is nine digits");
    exit(1);
  }

  clientServer();
  
  //initialize
  sensor = mraa_aio_init(1);
  if(sensor == NULL) {
    fprintf(stderr, "Error: Could not initialize AIO\n");
    exit(1);
  }
  
  atexit(closeSensors);
  getTemperature();

  return 0;
}
