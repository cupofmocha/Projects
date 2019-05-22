all: programs
	./dice 
	./cards

programs: dice.o cards.o
	gcc -o dice dice.o
	gcc -o cards cards.o

programs.o: dice.c cards.c
	gcc -c dice.c
	gcc -c cards.c

clean:
	rm -f programs *.o