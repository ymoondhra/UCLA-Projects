
default: clean
	chmod +x lab3b.sh
	-ln lab3b.sh lab3b

clean:	
	rm -f lab3b *.tar.gz *~

dist: default
	tar -cvzf lab3b-987654321.tar.gz lab3b.py lab3b.sh Makefile README 
