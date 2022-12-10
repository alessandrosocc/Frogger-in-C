home: home.o HighWay.o frog.o home.o
	gcc home.o HighWay.o frog.o -o home -lncurses
home.o: home.c
	gcc -c home.c -lncurses
HighWay.o: HighWay.c HighWay.h
	gcc -c HighWay.c -lncurses
frog.o: frog.c frog.h
	gcc -c frog.c -lncurses

clean:
	rm *.o
