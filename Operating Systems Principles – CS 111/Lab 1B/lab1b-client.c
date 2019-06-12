//NAME: Yash Moondhra
//Email: ymoondhra@gmail.com
//ID: 123456789

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
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <zlib.h>
#include <assert.h>

struct termios pastTerm;
int socket_fd;
int portNumber;
int p;
int l;
char* logfile;
int log_fd;
unsigned char parentBuffer[1000]; //1000 is the buffer size
//compression
int c;
unsigned char compressBuffer[1000]; //1000 is the buffer size
z_stream stream_in;
z_stream stream_out;

void reset(void) {
  //reset terminal attributes to original
  if(tcsetattr(STDIN_FILENO, TCSANOW, &pastTerm) < 0) {
    fprintf(stderr, "Error: could not set terminal attributes\n");
    exit(1);
  }
}

void pollFunction() {
  struct pollfd polls[2];
  polls[0].fd = 0;
  polls[1].fd = socket_fd;//pipe2[0];
  polls[1].events = POLLIN | POLLHUP | POLLERR;
  polls[0].events = POLLIN | POLLHUP | POLLERR;

  while(1) {
    int poller = poll(polls, 2, 0);
    if(poller < 0) {
      fprintf(stderr, "Error: could not poll");
      exit(1);
    }
    else {
        if(polls[0].revents & (POLLHUP | POLLERR)) {
            exit(0);
        }
        else if(polls[0].revents & POLLIN) {
            int numRead;
            numRead = read(0, &parentBuffer, 1000);
            if(numRead < 0) {
                fprintf(stderr, "Error: could not read from STDIN");
                exit(1);
            }
            if(c == 1) {
                stream_in.opaque = Z_NULL;
                stream_in.zfree = Z_NULL;
                stream_in.zalloc = Z_NULL;
                
                if(deflateInit(&stream_in, Z_DEFAULT_COMPRESSION) != Z_OK) {
                    fprintf(stderr, "Error: deflateInit failed");
                    exit(1);
                }

                stream_in.avail_in = numRead;
                stream_in.avail_out = 1000;
                stream_in.next_in = parentBuffer;
                stream_in.next_out = compressBuffer;
                
                do {
                    if(deflate(&stream_in, Z_SYNC_FLUSH) != Z_OK) {
                        fprintf(stderr, "Error: deflate failed");
                        exit(1);
                    }
                } while (stream_in.avail_in > 0);
                
                numRead = 1000 - stream_in.avail_out;
                write(socket_fd, &compressBuffer, numRead);

                if(l == 1) {
                    int x = 0;
                    while(x < numRead) {
                        char sent[] = "SENT 1 byte: ";
                        int wrote = write(log_fd, sent, 13);
                        if(wrote == -1) {
                            fprintf(stderr, "Error: could not write to log file");
                            exit(1);
                        }
			dprintf(log_fd, "%c\n", compressBuffer[x]);
                        x++;
                    }
                }
                deflateEnd(&stream_in);
            }
            int y = 0;
            while(y < numRead) {
                if(parentBuffer[y] == '\r' || parentBuffer[y] == '\n') {
                    char d = '\n';
                    write(1, &d, 1);
                    d = '\r';
                    write(1, &d, 1);
                    if(c != 1) {
                        write(socket_fd, &parentBuffer[y], 1);
                        if(l == 1) {
                            if(write(log_fd, "SENT 1 byte: ", 13) < 0) {
                                fprintf(stderr, "Error: could not write to log file");
                                exit(1);
                            }
			    dprintf(log_fd, "%c\n", parentBuffer[y]);
                            
                        }
                    }
                }
                else {
                    write(1, &parentBuffer[y], 1);
                    if(c != 1) {
                        write(socket_fd, &parentBuffer[y], 1);
                        if(l == 1) {
                            if(write(log_fd, "SENT 1 byte: ", 13) < 0) {
                                fprintf(stderr, "Error: could not write to log file");
                                exit(1);
                            }
                            if(write(log_fd, &parentBuffer[y], 1) < 0) {
                                fprintf(stderr, "Error: could not write to log file");
                      exit(1);
                  }
                  char d = '\n';
                  write(1, &d, sizeof(char));
              }
          }
	  }
	  y++;
	}
	memset(parentBuffer, 0, numRead);
      }
      if(polls[1].revents & POLLIN) {
	int num, i;
	num = read(socket_fd, &parentBuffer, 1000);
	if(num < 0) {
	  fprintf(stderr, "Error: could not read from SHELL");
	  exit(1);
	}
	else if(num == 0) {
	  exit(0);
	}
	if(c == 1) {
	  if(l == 1) {
	    for(i = 0; i < num; i++) {
	      if(write(log_fd, "RECEIVED 1 byte: ", 17) < 0)
		  {
		    fprintf(stderr, "Error: could not write to log file");
		    exit(1);
		  }
	      if(write(log_fd, &parentBuffer[i], 1) < 0)
		{
		  fprintf(stderr, "Error: could not write to log file");
		  exit(1);
		}
	      char d = '\n';
	      write(1, &d, sizeof(char));
	    }
	  }
	  stream_out.opaque = Z_NULL;
	  stream_out.zfree = Z_NULL;
	  stream_out.zalloc = Z_NULL;

	  if(inflateInit(&stream_out) != Z_OK) {
	    fprintf(stderr, "Error: inflateInit failed");
	    exit(1);
	  }

	  stream_out.avail_in = num;
	  stream_out.avail_out = 1000;
	  stream_out.next_in = parentBuffer;
	  stream_out.next_out = compressBuffer;

	  while(1) {
	    if(inflate(&stream_out, Z_SYNC_FLUSH)) {
	      fprintf(stderr, "Error: inflate failed");
	      exit(1);
	    }
	    if(stream_out.avail_in <= 0) {
	      break;
	    }
	  }
	  inflateEnd(&stream_out);

	  int difference = 1000 - stream_out.avail_out;
	  int q = 0;
	  for(; q < difference; q++) {
	    if(compressBuffer[q] == 13 || compressBuffer[q] == 10) {
	      compressBuffer[q] = 13;
	      write(1, &compressBuffer[q], 1);
	      compressBuffer[q] = 10;
	      write(1, &compressBuffer[q], 1);
	    }
	    else {
	      write(1, &compressBuffer[q], 1);
	    }
	  }
	}
	else {
	  int r = 0;
	  for(; r < num; r++) {
	    if(l == 1) {
	      if(write(log_fd, "RECEIVED 1 byte: ", 17) < 0) {
		  fprintf(stderr, "Error: could not write to log file");
		  exit(1);	
		}
	      if(write(log_fd, &parentBuffer[r], 1) < 0 ) {
		  fprintf(stderr, "Error: could not write to log file");
		  exit(1);
		}
	      char d = '\n';
	      write(log_fd, &d, 1); 
	    }
	    if(parentBuffer[r] == 13 || parentBuffer[r] == 10) {
	      parentBuffer[r] = 13;
	      write(1, &parentBuffer[r], 1);
	      parentBuffer[r] = 10;
	      write(1, &parentBuffer[r], 1);
	    }
	    else {
	      write(1, &parentBuffer[r], 1);
	    }
	  }
	  memset(parentBuffer, 0, num);
	}
      }
      else if(polls[1].revents & (POLLERR | POLLHUP)) {
          exit(0);
      }
    }
  }
}

void clientServer() {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1) {
        fprintf(stderr, "Error: could not create a socket\n");
        exit(1);
    }
    
    struct hostent * server = gethostbyname("127.0.0.1");
    if(server == NULL) {
        fprintf(stderr, "Error: could not get host name\n");
        exit(1);
    }
    
    if(l == 1) {
        log_fd = creat(logfile, 0666);
        if(log_fd < 0) {
            fprintf(stderr, "Error: could not open/create log file");
            exit(1);
        }
    }
    
    //NOTE: The following code was adapted from:
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); //resets the server_address struct
    //bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET; //sin_family is the ode for the address family
    server_address.sin_port = htons(portNumber); //sin_port has the port number. htons converts
    //a port number from host to network byte order
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "Error: could not connect server\n");
        exit(1);
    }
    
    pollFunction();
    
}



int main(int argc, char* argv[]) {
    p = 0;
    l = 0;
    portNumber = 0;
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"compress", no_argument, 0, 'c'},
        {0,0,0,0},
    };
    int optindex = 0;
    while(1) {
        int opt_character = getopt_long(argc,argv, "", long_options, &optindex);
        if(opt_character == '?') {
            fprintf(stderr, "Error: unrecognized argument. Use --port and/or --log");
            exit(1);
        }
    if(opt_character == 'p') {
      p = 1;
      portNumber = atoi(optarg);
    }
    else if(opt_character == 'l') {      
      l = 1;
      logfile = optarg;
    }
    else if(opt_character == 'c') {
      c = 1;
    }
    else if(opt_character == -1) {
      break;
    }
  }
    
    if(tcgetattr(STDIN_FILENO, &pastTerm) < 0) {
        fprintf(stderr, "Error: could not get terminal attributes\n");
        exit(1);
    }
    struct termios newTerm = pastTerm;
    newTerm.c_iflag = ISTRIP;
    newTerm.c_oflag = 0;
    newTerm.c_lflag = 0;
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    if(tcsetattr(STDIN_FILENO, TCSANOW, &newTerm) < 0) {
        fprintf(stderr, "Error: could not set terminal attributes\n");
        exit(1);
    }

  if(p) {
      clientServer();
  }
  else {
      fprintf(stderr, "Error: you must pass in a port number using --port");
      exit(1);
  }
    
 
  //SOCKET
}
