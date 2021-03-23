CFLAGS =-Wall

allocate: main.c
	gcc $(CFLAGS) -o allocate main.c

.PHONY: clean

clean:
	rm -f allocate