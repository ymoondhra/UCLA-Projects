#NAME: Shubhrose Singh, Yash Moondhra
#EMAIL: shubhroses@gmail.com, ymoondhra@gmail.com
#ID: 987654321, 123456789

default:
	gcc -g -Wall -Wextra lab3a.c -o lab3a

dist: clean default
	tar -cvzf lab3a-987654321.tar.gz lab3a.c ext2_fs.h Makefile README

clean:
	rm -f lab3a lab3a-987654321.tar.gz
