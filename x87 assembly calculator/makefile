all: calc

calc: calc.o functions.o

	gcc -shared -g -no-pie -Wall -o calc calc.o functions.o

calc.o: calc.c

	gcc -shared -g -fPIC -Wall -c -o calc.o calc.c

functions.o: functions.s

	nasm -g -f elf64 -w+all -shared -o functions.o functions.s

.PHONY: clean
	rm -f *.o calc
