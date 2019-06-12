#!/bin/bash

echo "Testing Bad Argument..."
# Check Bad Argument
perfect=1
echo | ./lab4b --bad &> /dev/null; \
if [[ $? -ne 1 ]]; then \
	echo "ERROR: passing in unsupported option failed\n"
	$perfect=0
fi

echo "Testing All Options..."
# Test All Options
rm -f log364513.txt #random name for log file
./lab4b --log="log364513.txt" --period=3 --scale="C" <<-EOF
OFF
EOF
if [[ $? -ne 0 ]]; then \
	echo "ERROR: not all options are supported: --log, --period, --scale\n"
	$perfect=0
fi

if [ ! -f log364513.txt ]; then \
	echo "ERROR: log file was not created\n"
	$perfect=0
fi

echo "Testing Input..."
# Test Input
./lab4b --log="log364513.txt" --period=3 --scale="C" <<-EOF
STOP
START
SCALE=F
SCALE=C
LOG HELLO 23
OFF
EOF
returned=$?

echo "Testing Returned Value..."
if [ $returned -ne 0 ]; then \
	echo "ERROR: Input to program failed"
	$perfect=0
fi

echo "Testing Log File Errors..."
logfileErrors=0
grep STOP log364513.txt > /dev/null
let "logFileErrors+=$?"
grep START log364513.txt > /dev/null
let "logFileErrors+=$?"
grep SCALE=F log364513.txt > /dev/null
let "logFileErrors+=$?"
grep SCALE=C log364513.txt > /dev/null
let "logFileErrors+=$?"
grep LOG log364513.txt > /dev/null
let "logFileErrors+=$?"
grep OFF log364513.txt > /dev/null
let "logFileErrors+=$?"
grep SHUTDOWN log364513.txt > /dev/null
let "logFileErrors+=$?"

if [ $logfileErrors -ne 0 ]; then \
	echo "ERROR: log file did not log $logFileErrors commands\n"
	$perfect=0
fi

if [ $perfect -eq 1 ]; then \
	echo "SUCCESS: All tests were passed successfully\n"
fi

rm -f log364513.txt