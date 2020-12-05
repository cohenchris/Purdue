SRC=../my_malloc.c ../printing.c
GCC=gcc -g -std=gnu11 -Wall -Werror -I"/homes/cs252/public/include" 
BENCH_SRC_PATH="/homes/cs252/public/bench"

.PHONY: testall
testall: clean test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test14
		@cd .. && make

.PHONY: testPart1
testPart1: clean test1 test2 test3 test7 test9 test10 test11
		@cd .. && make

.PHONY: clean
clean:
		rm -f test log.txt Output/*
		clear

.PHONY: deepclean
deepclean: clean
		rm -f *.o
		clear

.PHONY: bench
bench: my_malloc.o
		for file in ${BENCH_SRC_PATH}/*.o; do \
			echo "Running benchmark: $$file"; \
			${GCC} my_malloc.o $$file -lpthread -o bench; \
			./bench && echo; \
		done

.PHONY: test1
test1: test1.o test1_m32.o SRC_F
		@${GCC} test1.o my_malloc.o printing.o -o test
		@bash run_test.sh 1 && echo "Test 1 \e[92mPASSED\e[0m" || echo "Test 1 \e[91mFAILED\e[0m"
		@${GCC} -m32 test1_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 1-m32 && echo "Test 1-m32 \e[92mPASSED\e[0m" || echo "Test 1-m32 \e[91mFAILED\e[0m"

.PHONY: test2
test2:
		@${GCC} test2.c ${SRC} -o test
		@bash run_test.sh 2-DefaultArena && echo "Test 2-DefaultArena \e[92mPASSED\e[0m" || echo "Test 2-DefaultArena \e[91mFAILED\e[0m"
		@${GCC} test2.c ${SRC} -DARENA_SIZE=1024 -o test
		@bash run_test.sh 2-SmallArena && echo "Test 2-SmallArena \e[92mPASSED\e[0m" || echo "Test 2-SmallArena \e[91mFAILED\e[0m"
		@${GCC} -m32 test2.c ${SRC} -o test
		@bash run_test.sh 2-DefaultArena-m32 && echo "Test 2-DefaultArena-m32 \e[92mPASSED\e[0m" || echo "Test 2-DefaultArena-m32 \e[91mFAILED\e[0m"
		@${GCC} -m32 test2.c ${SRC} -DARENA_SIZE=1024 -o test
		@bash run_test.sh 2-SmallArena-m32 && echo "Test 2-SmallArena-m32 \e[92mPASSED\e[0m" || echo "Test 2-SmallArena-m32 \e[91mFAILED\e[0m"

.PHONY: test3
test3: SRC_F
		@${GCC} -Wl,--wrap=sbrk test3.c my_malloc.o printing.o -o test
		@bash run_test.sh 3 && echo "Test 3 \e[92mPASSED\e[0m" || echo "Test 3 \e[91mFAILED\e[0m"
		@${GCC} -m32 -Wl,--wrap=sbrk test3.c printing_m32.o my_malloc_m32.o -o test
		@bash run_test.sh 3-m32 && echo "Test 3-m32 \e[92mPASSED\e[0m" || echo "Test 3-m32 \e[91mFAILED\e[0m"

.PHONY: test4
test4: test4.o test4_m32.o SRC_F
		@${GCC} test4.o my_malloc.o printing.o -o test
		@bash run_test.sh 4 && echo "Test 4 \e[92mPASSED\e[0m" || echo "Test 4 \e[91mFAILED\e[0m"
		@${GCC} -m32 test4_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 4-m32 && echo "Test 4-m32 \e[92mPASSED\e[0m" || echo "Test 4-m32 \e[91mFAILED\e[0m"

.PHONY: test5
test5: test5.o test5_m32.o SRC_F
		@${GCC} test5.o my_malloc.o printing.o -o test
		@bash run_test.sh 5 should_fail && echo "Test 5 \e[92mPASSED\e[0m" || echo "Test 5 \e[91mFAILED\e[0m"
		@${GCC} -m32 test5_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 5-m32 should_fail && echo "Test 5-m32 \e[92mPASSED\e[0m" || echo "Test 5-m32 \e[91mFAILED\e[0m"

.PHONY: test6
test6: test6.o test6_m32.o SRC_F
		@${GCC} test6.o my_malloc.o printing.o -o test
		@bash run_test.sh 6 && echo "Test 6 \e[92mPASSED\e[0m" || echo "Test 6 \e[91mFAILED\e[0m"
		@${GCC} -m32 test6_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 6-m32 && echo "Test 6-m32 \e[92mPASSED\e[0m" || echo "Test 6-m32 \e[91mFAILED\e[0m"

.PHONY: test7
test7: test7.o test7_m32.o SRC_F
		@${GCC} test7.o my_malloc.o printing.o -o test
		@bash run_test.sh 7 && echo "Test 7 \e[92mPASSED\e[0m" || echo "Test 7 \e[91mFAILED\e[0m"
		@${GCC} -m32 test7_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 7-m32 && echo "Test 7-m32 \e[92mPASSED\e[0m" || echo "Test 7-m32 \e[91mFAILED\e[0m"

.PHONY: test8
test8: test8.o test8_m32.o SRC_F
		@${GCC} test8.o my_malloc.o printing.o -o test
		@bash run_test.sh 8 && echo "Test 8 \e[92mPASSED\e[0m" || echo "Test 8 \e[91mFAILED\e[0m"
		@${GCC} -m32 test8_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 8-m32 && echo "Test 8-m32 \e[92mPASSED\e[0m" || echo "Test 8-m32 \e[91mFAILED\e[0m"

.PHONY: test9
test9:
		@${GCC} test9.c ${SRC} -o test
		@bash run_test.sh 9-DefaultMinAlloc && echo "Test 9-DefaultMinAlloc \e[92mPASSED\e[0m" || echo "Test 9-DefaultMinAlloc \e[91mFAILED\e[0m"
		@${GCC} test9.c ${SRC} -DMIN_ALLOCATION=16 -o test
		@bash run_test.sh 9-LargeMinAlloc && echo "Test 9-LargeMinAlloc \e[92mPASSED\e[0m" || echo "Test 9-LargeMinAlloc \e[91mFAILED\e[0m"
		@${GCC} -m32 test9.c ${SRC} -o test
		@bash run_test.sh 9-DefaultMinAlloc-m32 && echo "Test 9-DefaultMinAlloc-m32 \e[92mPASSED\e[0m" || echo "Test 9-DefaultMinAlloc-m32 \e[91mFAILED\e[0m"
		@${GCC} -m32 test9.c ${SRC} -DMIN_ALLOCATION=16 -o test
		@bash run_test.sh 9-LargeMinAlloc-m32 && echo "Test 9-LargeMinAlloc-m32 \e[92mPASSED\e[0m" || echo "Test 9-LargeMinAlloc-m32 \e[91mFAILED\e[0m"

.PHONY: test10
test10: test10.o test10_m32.o SRC_F
		@${GCC} test10.o my_malloc.o printing.o -o test
		@bash run_test.sh 10 && echo "Test 10 \e[92mPASSED\e[0m" || echo "Test 10 \e[91mFAILED\e[0m"
		@${GCC} -m32 test10_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 10-m32 && echo "Test 10-m32 \e[92mPASSED\e[0m" || echo "Test 10-m32 \e[91mFAILED\e[0m"

.PHONY: test11
test11: test11.o test11_m32.o SRC_F
		@${GCC} test11.o my_malloc.o printing.o -o test
		@bash run_test.sh 11 && echo "Test 11 \e[92mPASSED\e[0m" || echo "Test 11 \e[91mFAILED\e[0m"
		@${GCC} -m32 test11_m32.o my_malloc_m32.o printing_m32.o -o test
		@bash run_test.sh 11-m32 && echo "Test 11-m32 \e[92mPASSED\e[0m" || echo "Test 11-m32 \e[91mFAILED\e[0m"

.PHONY: test12
test12: test12.o test12_m32.o
		@${GCC} test12.o ${SRC} -DFIT_ALGORITHM=1 -o test
		@bash run_test.sh 12-First && echo "Test 12-First \e[92mPASSED\e[0m" || echo "Test 12-First \e[91mFAILED\e[0m"
		@${GCC} test12.o ${SRC} -DFIT_ALGORITHM=3 -o test
		@bash run_test.sh 12-Best && echo "Test 12-Best \e[92mPASSED\e[0m" || echo "Test 12-Best \e[91mFAILED\e[0m"
		@${GCC} test12.o ${SRC} -DFIT_ALGORITHM=4 -o test
		@bash run_test.sh 12-Worst && echo "Test 12-Worst \e[92mPASSED\e[0m" || echo "Test 12-Worst \e[91mFAILED\e[0m"
		@${GCC} -m32 test12_m32.o ${SRC} -DFIT_ALGORITHM=1 -o test
		@bash run_test.sh 12-First-m32 && echo "Test 12-First-m32 \e[92mPASSED\e[0m" || echo "Test 12-First-m32 \e[91mFAILED\e[0m"
		@${GCC} -m32 test12_m32.o ${SRC} -DFIT_ALGORITHM=3 -o test
		@bash run_test.sh 12-Best-m32 && echo "Test 12-Best-m32 \e[92mPASSED\e[0m" || echo "Test 12-Best-m32 \e[91mFAILED\e[0m"
		@${GCC} -m32 test12_m32.o ${SRC} -DFIT_ALGORITHM=4 -o test
		@bash run_test.sh 12-Worst-m32 && echo "Test 12-Worst-m32 \e[92mPASSED\e[0m" || echo "Test 12-Worst-m32 \e[91mFAILED\e[0m"

.PHONY: test13
test13: test13.o test13_m32.o
		@${GCC} test13.o ${SRC} -DFIT_ALGORITHM=2 -o test
		@bash run_test.sh 13 && echo "Test 13 \e[92mPASSED\e[0m" || echo "Test 13 \e[91mFAILED\e[0m"
		@${GCC} -m32 test13_m32.o ${SRC} -DFIT_ALGORITHM=2 -o test
		@bash run_test.sh 13-m32 && echo "Test 13-m32 \e[92mPASSED\e[0m" || echo "Test 13-m32 \e[91mFAILED\e[0m"

.PHONY: test14
test14: test14.o test14_m32.o
		@${GCC} test14.o ${SRC} -DFIT_ALGORITHM=2 -o test
		@bash run_test.sh 14 && echo "Test 14 \e[92mPASSED\e[0m" || echo "Test 14 \e[91mFAILED\e[0m"
		@${GCC} -m32 test14_m32.o ${SRC} -DFIT_ALGORITHM=2 -o test
		@bash run_test.sh 14-m32 && echo "Test 14-m32 \e[92mPASSED\e[0m" || echo "Test 14-m32 \e[91mFAILED\e[0m"


test%.o: test%.c
		@${GCC} $< -c -o $@

test%_m32.o: test%.c
		@${GCC} -m32 $< -c -o $@

my_malloc.o: ../my_malloc.c
		@${GCC} $< -c -o $@

my_malloc_m32.o: ../my_malloc.c
		@${GCC} -m32 $< -c -o $@

printing.o: ../printing.c
		@${GCC} $< -c -o $@

printing_m32.o: ../printing.c
		@${GCC} -m32 $< -c -o $@

.PHONY: SRC_F
SRC_F: my_malloc.o printing.o printing_m32.o my_malloc_m32.o

