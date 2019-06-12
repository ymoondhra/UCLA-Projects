#NAME: Yash Moondhra                                                            
#EMAIL: ymoondhra@gmail.com                                                     
#ID: 123456789

default: build

build: clean lab4b.c
	gcc -lmraa -lm -Wall -Wextra -g lab4b.c -o lab4b

clean:
	rm -f *tar.gz lab4b

check: build
	chmod +x lab4b-test.sh
	./lab4b-test.sh

dist: lab4b.c Makefile README
	tar -czvf lab4b-123456789.tar.gz lab4b.c Makefile README lab4b-test.sh
