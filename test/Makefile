CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = myGit test 

.PHONY: all clean 

all: $(PROGRAMS)

test: bash.o src.o main.o
	$(CC) -o $@ $(CFLAGS) $^

myGit: bash.o src.o myGit.o
	$(CC) -o $@ $(CFLAGS) $^

src.o: src.c 
	gcc $(CFLAGS) -c src.c

bash.o: bash.c 
	gcc $(CFLAGS) -c bash.c

myGit.o: myGit.c 
	gcc $(CFLAGS) -c myGit.c 

main.o: main.c 
	gcc $(CFLAGS) -c main.c 

clean:
	rm -f *.o *~ $(PROGRAMS)