#NAME: Yash Moondhra                                                            
#EMAIL: ymoondhra@gmail.com                                                     
#ID: 123456789

default: build

build: clean lab4c_tcp.c
	gcc -lmraa -lm -Wall -Wextra -g lab4c_tcp.c -o lab4c_tcp
	gcc -lmraa -lm -Wall -Wextra -g lab4c_tls.c -o lab4c_tls -lssl -lcrypto

clean:	
	rm -f *tar.gz lab4c_tcp lab4c_tls

dist: build
	tar -czvf lab4c-123456789.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README
