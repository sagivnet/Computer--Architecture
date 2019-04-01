all: root

root: roots.o

	gcc -shared -g -no-pie -Wall -o root roots.o

roots.o: roots.s

	nasm -g -f elf64 -w+all -shared -o roots.o roots.s

.PHONY: clean
	rm -f *.o roots
