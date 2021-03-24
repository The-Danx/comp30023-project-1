CFLAGS =-Wall

allocate: main.o list.o
	gcc $(CFLAGS) -o allocate main.o list.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

.PHONY: clean

clean:
	rm -f allocate