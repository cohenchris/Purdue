
test_num=$1
should_fail=$2

timeout=10

echo >> log.txt

#	Run the test redirecting all output to log.txt
#	Weird logic is to prevent SIGABRT from causing bad printing in Makefile
!(./test &> output/$test_num.txt || false) &>/dev/null &
pid=$!

#	Let the process run
kill -0 $pid &> /dev/null
process_live=$?
while (( process_live == 0 )); do
	if ((timeout == 0)); then
		#	If it takes too long, kill it
		kill $pid &> /dev/null
		echo "Test ${test_num}: FAIL (timeout)" >> log.txt
		echo "End ${test_num}" >> log.txt
		exit 1
	fi
	kill -0 $pid &> /dev/null
	process_live=$?
	sleep 1
	((timeout--))
done

#	If the process completes, check whether it ran successfully or not
wait $pid &> /dev/null
success=$?

#	If the test has an expected output, diff that 
if [ -e expected/${test_num}.txt ]
then
	#	Run a diff that only compares lines that can be found in Expected.
	#	Allows us to remove non-deterministic lines
	diffFail=$(diff --new-line-format="" --unchanged-line-format="" expected/${test_num}.txt output/${test_num}.txt | tr -d [:space:] | wc -c)
	if (( $diffFail != 0 ))
	then
		echo "Test ${test_num}: FAIL (diff)" >> log.txt
		exit 1
	fi
fi

#	Make sure the return code is what it should be
if [ "$should_fail" = "should_fail" ]
then
	if (( ${success} == 0 ))
	then
		echo "Test ${test_num}: FAIL (program should have failed an assert, but did not)" >> log.txt
		exit 1
	else
		echo "Test ${test_num}: SUCCESS" >> log.txt
		exit 0
	fi
else
	if (( ${success} != 0 ))
	then
		echo "Test ${test_num}: FAIL (program failed an assert, but should not have)" >> log.txt
		exit 1
	else
		echo "Test ${test_num}: SUCCESS" >> log.txt
		exit 0
	fi
fi
