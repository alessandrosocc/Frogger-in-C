
home: home.o HighWay.o frog.o river.o menuGenerator.o
	gcc home.o HighWay.o frog.o river.o menuGenerator.o -o home -lncurses
home.o: home.c
	gcc -c home.c -lncurses
HighWay.o: HighWay.c HighWay.h
	gcc -c HighWay.c -lncurses
frog.o: frog.c frog.h
	gcc -c frog.c -lncurses
river.o: river.c river.h
	gcc -c river.c -lncurses
menuGenerator.o: menuGenerator.c
	gcc -c menuGenerator.c
clean:
	rm *.o



