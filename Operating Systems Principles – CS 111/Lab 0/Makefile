#NAME: Yash Moondhra
#EMAIL: ymoondhra@gmail.com
#ID: 123456789

default:
	gcc -Wall -Wextra -g lab0.c -o lab0

check:
	#checking --input and --output
	`echo "hello and goodbye" > x23yz.txt` 
	`./lab0 --input=x23yz.txt --output=x24yz.txt`
	if [[ $$? -eq 0 ]] ; then \
		echo "Success: returned 0 with the --input and --output options"; \
	else \
		echo "Failure: --input and/or --output does not function properly. Return value: $$?"; \
	fi

	#making sure output is identical to input
	`cmp x23yz.txt x24yz.txt`
	if [[ $$? -eq 0 ]] ; then \
		echo "Success: the input and output files are identical"; \
	else \
		echo "Failure: the input file is not identical to the output file"; \
	fi

	#input file cannot be accessed 
	`chmod -wr x23yz.txt`; \
	`./lab0 --input=x23yz.txt --output=x24yz.txt`; \
	if [[ $$? -eq 2 ]] ; then \
		echo "Success: returned 2 due to lack of permission to access the  --input file"; \
	else \
		echo "Failure: did not exit with return value 2 even though lacked permission to read --input file. Return value: $$?"; \
	fi

	#output file cannot be accessed
	`./lab0 --input=x24yz.txt --output=x23yz.txt`; \
	if [[ $$? -eq 3 ]] ; then \
		echo "Success: returned 3 due to lack of permission to write to the --output file"; \
	else \
		echo "Failure: did not exit with return value 3 even though lacked permission to write to --output file. Instead returned $$?"; \
	fi

	`chmod +wr x23yz.txt`; \
	`chmod +wr x24yz.txt`; \

	#catching the segfault
	`./lab0 --segfault --catch`;\
	if [[ $$? -eq 4 ]] ; then \
		echo "Success: returned 4 on segfault because of catch"; \
	else \
		echo "Failure: did not return 4 on segfault with --catch option. Instead returned $$?"; \
	fi

clean:
	rm -f lab0-123456789.tar.gz lab0 x23yz.txt x24yz.txt

dist:
	tar -cvzf lab0-123456789.tar.gz lab0.c backtrace.png breakpoint.png README Makefile
