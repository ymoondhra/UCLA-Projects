/*
NAME: Yash Moondhra
MAIL: ymoondhra@gmail.com
UID: 123456789
 */

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <signal.h>
#include <assert.h>
#include <zlib.h>

int pipe1[2];
int pipe2[2];
pid_t cpid;

//port/socket variables
int p; //port option (initialized to 0)
int portNumber;
unsigned char parentBuffer[1000];
unsigned char compressBuffer[1000];

//compression variables
int c; //compression option (initialized to 0)
int new_socket_fd;
z_stream stream_in;
z_stream stream_out;

void reset(void) {
  int status;
  //wait for child (shell) task to complete
  if(waitpid(cpid, &status, 0) < 0) {
       fprintf(stderr, "Error: child process not completed");
       exit(1);
  }
  //output the appropriate error code
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
  exit(0);
}

void signalHandler(int signal_number) {
  if(signal_number == SIGPIPE) {
    fprintf(stderr, "Signal code: SIGPIPE"); 
  }
  else if(signal_number == SIGINT) {
    fprintf(stderr, "Signal code: SIGINT");
  }
}

void runShell(int socket_fd) {
	if(pipe(pipe1) == -1 || pipe(pipe2) == -1) {
	   fprintf(stderr, "Error: could not create pipe");
	   exit(1);
	}
	cpid = fork();
	if(cpid < 0) {
		fprintf(stderr, "Error: could not fork");
		exit(1);
	}
	else if(cpid == 0) {
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
	else {
	    close(pipe1[0]);
	    close(pipe2[1]);

	    struct pollfd polls[2];
	    polls[0].fd = new_socket_fd;
	    polls[1].fd = pipe2[0];
	    polls[1].events = POLLIN | POLLHUP | POLLERR;
	    polls[0].events = POLLIN | POLLHUP | POLLERR;
		
		while(1) {
            memset(compressBuffer, 0, 1000);
            memset(parentBuffer, 0, 1000);
			if(poll(polls, 2, -1) < 0) {
				fprintf(stderr,"Error: poll failed");	
				exit(1);
			}
			else if (polls[0].revents & POLLIN) {
				int numRead = read(new_socket_fd, &parentBuffer, 1000);
				if(numRead < 0) {
					fprintf(stderr,"Error: read failed from client");	
					exit(1);
				}
                
                unsigned char* buff = parentBuffer;

				if(c == 1) {
                    stream_out.zfree = Z_NULL;
                    stream_out.zalloc = Z_NULL;
					stream_out.opaque = Z_NULL;
					if (inflateInit(&stream_out) != Z_OK) {
						fprintf(stderr,"Error: inflateInit failed");	
						exit(1);
					}
                    stream_out.avail_out = 1000;
					stream_out.avail_in = numRead;
                    stream_out.next_out = compressBuffer;
					stream_out.next_in = parentBuffer;
                    
                    do {
                        if (inflate(&stream_out, Z_SYNC_FLUSH) != Z_OK) {
                            fprintf(stderr,"Error: inflate failed");
                            exit(1);
                        }
                    } while (stream_out.avail_in > 0);
					
					inflateEnd(&stream_out);
					numRead = 1000 - stream_out.avail_out;
                    buff = compressBuffer;
                    int i = 0;
					while(i < numRead) {
						if(buff[i] == '\03') {
							kill(cpid, SIGINT);
						}
                        else if(buff[i] == '\04') {
                            close(pipe1[1]);
                            close(socket_fd);
                            close(pipe2[0]);
                            close(new_socket_fd);
                            exit(0);
                        }
						else if(buff[i] == '\n' || buff[i] == '\r') {
							char d = '\n';
							write(pipe1[1], &d, 1);
						}
						else {
							write(pipe1[1], &buff[i],1);
						}
                        i++;
					}
				}
				else {
                    int i = 0;
					while(i < numRead) {
						if(buff[i] == '\04') {
							close(pipe1[1]);
							close(socket_fd);
                            close(pipe2[0]);
							close(new_socket_fd);
							exit(0);
						}
						else if(buff[i] == '\03') {
							kill(cpid, SIGINT);
						}
						else if(buff[i] == '\n' || buff[i] == '\r') {
							char d = '\n';
							write(pipe1[1], &d, 1);
						}
						else {
							write(pipe1[1], &buff[i],1);
						}
                        i++;
					}
				}
			}
            memset(compressBuffer, 0, 1000);
            memset(parentBuffer, 0, 1000);
			if(polls[1].revents & POLLIN) {
				int numRead = read(pipe2[0], &parentBuffer, 1000);
				if(numRead < 0) {
					fprintf(stderr,"Error: read failed from shell");	
					exit(1);
				}

				if(c == 1) {
                    stream_in.zfree = Z_NULL;
                    stream_in.zalloc = Z_NULL;
					stream_in.opaque = Z_NULL;
					
					if (deflateInit(&stream_in, Z_DEFAULT_COMPRESSION) != Z_OK){
						fprintf(stderr,"Error: deflateInit failed");	
						exit(1);
					}

					stream_in.avail_in = numRead;
					stream_in.avail_out = 1000;
					stream_in.next_in = parentBuffer;
					stream_in.next_out = compressBuffer;
					
                    do {
                        if (deflate(&stream_in, Z_SYNC_FLUSH) != Z_OK)
                        {
                            fprintf(stderr,"Error: deflate failed");
                            exit(1);
                        }
                    } while(stream_in.avail_in > 0);
                    
					numRead = 1000 - stream_in.avail_out;
					write(new_socket_fd, &compressBuffer, numRead);
					
					deflateEnd(&stream_in);
				}
				else {
                    int i = 0;
					while(i < numRead) {
                        if(parentBuffer[i] == '\r' || parentBuffer[i] == '\n') {
							parentBuffer[i] = '\n';
							write(new_socket_fd,&parentBuffer[i],1);
						}
						else {
							write(new_socket_fd,&parentBuffer[i],1);
						}
                        i++;
					}
                
				}
			}
			else if (polls[1].revents & (POLLHUP | POLLERR)) {
				exit(1);
			}
		}
	}
}

int main(int argc, char* argv[])
{
  atexit(reset);
  p = 0;
  c  = 0;

  static struct option long_options[] = {
    {"port", required_argument, 0, 'p'},
    {"compress", no_argument, 0, 'c'},
    {0,0,0,0},
  };

  int optindex = 0;
  while(1) {
    int opt_character = getopt_long(argc,argv, "", long_options, &optindex);

    if(opt_character == 'p') {
      p = 1;
      portNumber = atoi(optarg);
      signal(SIGINT, signalHandler);
      signal(SIGINT, signalHandler);
    }
    else if(opt_character == 'c') {
      c = 1;
    }
    else if(opt_character == -1) {
      break;
    }
    else if(opt_character == '?') {
      fprintf(stderr, "Error: unrecognized argument. Please use --port and/or --compress");
      exit(1);
    }
    else {
      fprintf(stderr, "Error: unrecognized argument. Please use --port and/or --compress");
      exit(1);
    }
  }
	int socket_fd;
  socklen_t client_length;
  struct sockaddr_in server_address, client_address;
  
  if(argc < 2) {
    fprintf(stderr, "Error: no port provided.\n");
    exit(1);
  }


  //------------------------- SOCKET
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd == -1) {
    fprintf(stderr, "Error: cannot open socket");
    exit(1);
  }
  memset((char *) &server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(portNumber);
  if(bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
    fprintf(stderr, "Error: could not bind the socket file descriptor to the socket");
    exit(1);
  }
  listen(socket_fd, 5);
  client_length = sizeof(client_address);

  new_socket_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_length);
  if(new_socket_fd < 0) {
    fprintf(stderr, "Error: could not accept");
    exit(1);
  }

  runShell(socket_fd);
	return 0;
}
