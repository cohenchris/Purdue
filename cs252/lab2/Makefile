SRC=my_malloc.c printing.c
GCC=gcc -g -std=gnu11 -Wall -I"/homes/cs252/public/include"

.PHONY: compile_and_push
compile_and_push:
	@#	Compile the source file(s). If successful, push the changes
	@($(GCC) -c $(SRC) && \
	(git add $(SRC); \
	git commit -m "Latest build"; \
	git push) && \
	echo "Successfully recorded changes") || echo "Errors detected: no commit made. Ensure your code compiles and that you are in a valid git repo."

.PHONY: submit_part1
submit_part1:
	@git tag -fa v1.0 -m "Part 1"
	@git push -f --tags && \
	echo "Part 1 submission complete" || \
	echo "Errors detected: submission NOT complete. Ensure that you are in a valid git repo."

.PHONY: submit_final
submit_final:
	@git tag -fa v2.0 -m "Final"
	@git push -f --tags && \
	echo "Final submission complete" || \
	echo "Errors detected: submission NOT complete. Ensure that you are in a valid git repo."

.PHONY: clean
clean:
	rm -f *.o
