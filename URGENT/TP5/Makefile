CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = test 

.PHONY: all clean 

all: $(PROGRAMS)

test: cell.o src.o main.o
	$(CC) -o $@ $(CFLAGS) $^

cell.o: cell.c 
	gcc $(CFLAGS) -c cell.c

src.o: src.c 
	gcc $(CFLAGS) -c src.c

main.o: main.c 
	gcc $(CFLAGS) -c main.c 

clean:
	rm -f *.o *~ $(PROGRAMS)