CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = work cell bash commit myGit

.PHONY: all clean 

all: $(PROGRAMS)

myGit: bash.o src.o myGit.o myGit_casebreak.o
	$(CC) -o $@ $(CFLAGS) $^

bash: bash.o src.o bash_test.o
	$(CC) -o $@ $(CFLAGS) $^

commit: bash.o src.o commit_test.o
	$(CC) -o $@ $(CFLAGS) $^

work : bash.o src.o work_test.o
	$(CC) -o $@ $(CFLAGS) $^

cell : bash.o src.o cell_test.o
	$(CC) -o $@ $(CFLAGS) $^

src.o: src.c 
	gcc $(CFLAGS) -c src.c

bash.o: bash.c 
	gcc $(CFLAGS) -c bash.c

cell_test.o: cell_test.c 
	gcc $(CFLAGS) -c cell_test.c 

work_test.o: work_test.c 
	gcc $(CFLAGS) -c work_test.c 

bash_test.o: bash_test.c 
	gcc $(CFLAGS) -c bash_test.c 

commit_test.o: commit_test.c 
	gcc $(CFLAGS) -c commit_test.c 

myGit.o: myGit.c 
	gcc $(CFLAGS) -c myGit.c 

myGit_casebreak.o: myGit_casebreak.c 
	gcc $(CFLAGS) -c myGit_casebreak.c 

clean:
	rm -f *.o *~ $(PROGRAMS)