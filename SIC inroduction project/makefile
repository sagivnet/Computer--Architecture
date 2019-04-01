all: sic

sic: sic.o

	gcc -shared -g -no-pie -Wall -o sic sic.o

sic.o: sic.s

	nasm -g -f elf64 -w+all -shared -o sic.o sic.s

.PHONY: clean
	rm -f *.o sic
