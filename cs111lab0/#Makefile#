files = lab0.c Makefile README *.png

default:
	gcc -o lab0 -g lab0.c
check: default tests
	if [[ -s bad.txt ]]; then \
	  echo "Failed"; \
	  cat bad.txt; \
	  rm -f bad.txt; \
	else \
	  echo "All tests passed."; \
	fi 
clean: 
	rm -f *.o lab0  *.tar.gz

dist: default
	tar -czf lab0-104638411.tar.gz $(files)
tests:
	echo "Hello World" > input.txt; \
	./lab0 -i input.txt -o output.txt; \
	cmp input.txt output.txt; \
	if [[ $$? -ne 0 ]]; then \
	echo "Error: input and output not same" >> bad.txt; \
	fi; \
	rm -f input.txt output.txt; \
	echo "Hello World" > input.txt; \
	chmod u-r input.txt; \
	./lab0 -i input.txt -o output.txt; \
	if [[ $$? -ne 1 ]]; then \
	echo "Error: input not readable but no error" >> bad.txt; \
	fi; \
	rm -f input.txt output.txt; \
	echo "Hello World" > input.txt; \
	touch output.txt; \
	chmod u-w output.txt; \
	./lab0 -i input.txt -o output.txt; \
	if [[ $$? -ne 2 ]]; then \
	echo "Error: output not writable but no error" >> bad.txt; \
	fi; \
	rm -f input.txt output.txt; \
	echo "Hello World" > input.txt; \
	./lab0 -i input.txt -s -c -o output.txt; \
	if [[ $$? -ne 3 ]]; then \
	echo "Errror: did not catch seg fault" >> bad.txt; \
	fi; \
	rm -f input.txt output.txt;
