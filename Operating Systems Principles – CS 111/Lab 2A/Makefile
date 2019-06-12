#NAME: Yash Moondhra                                                            
#EMAIL: ymoondhra@gmail.com                                                     
#ID: 123456789

default: build

build: clean lab2_list.c lab2_add.c
	gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add
	gcc -Wall -Wextra -pthread SortedList.c lab2_list.c -o lab2_list
clean:
	rm -f lab2a-123456789.tar.gz lab2_add lab2_list

dist: graphs
	tar -zcvf lab2a-123456789.tar.gz Makefile README lab2_list.gp lab2_add.gp *.png lab2_add.csv lab2_list.csv SortedList.c lab2_list.c lab2_add.c SortedList.h

graphs: tests
	./lab2_add.gp 2> /dev/null
	./lab2_list.gp 2> /dev/null

tests: clean build
	echo "" > lab2_add.csv
	echo "" > lab2_list.csv
	./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=10 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=10 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=10 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=10 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=20 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=40 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=80 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=12 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=3 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=3 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=5 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=5 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=6 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=6 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=7 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=7 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=3 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=3 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=5 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=5 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=6 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=6 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=7 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=7 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=3 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=3 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=5 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=5 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=6 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=6 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=7 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=7 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=3 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=3 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=5 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=5 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=6 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=6 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=7 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=7 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=100000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=2 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=4 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=8 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=m --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --yield --sync=c --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --yield --sync=s --threads=12 --iterations=1000 >> lab2_add.csv
	./lab2_add --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=m --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=s --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=c --threads=1 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=m --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=s --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=c --threads=2 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=m --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=s --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=c --threads=4 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=m --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=s --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=c --threads=8 --iterations=10000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=m --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=s --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_add --sync=c --threads=12 --iterations=10000 >> lab2_add.csv
	./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv
	-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=10 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=100 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=10 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=100 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=10 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=100 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=10 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=100 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=1 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=2 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=4 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=8 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=16 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=2 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=2 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=2 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=2 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=2 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=4 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=4 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=4 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=4 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=4 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=8 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=8 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=8 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=8 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=8 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --threads=12 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=i --threads=12 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=d --threads=12 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=il --threads=12 --iterations=32 >> lab2_list.csv 2>/dev/null
	-./lab2_list --yield=dl --threads=12 --iterations=32 >> lab2_list.csv 2>/dev/null
	./lab2_list --sync=m --yield=i --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=2 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=4 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=8 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=12 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=16 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=20 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=24 >> lab2_list.csv
	./lab2_list --sync=m --yield=i --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=i --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=d --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=d --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=l --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=l --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=il --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=il --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=dl --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=dl --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=id --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=id --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=m --yield=idl --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --sync=s --yield=idl --threads=12 --iterations=32 >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=3 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=3 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=5 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=5 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=7 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=7 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=9 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=9 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

