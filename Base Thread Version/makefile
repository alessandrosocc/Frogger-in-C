home: home.o frog.o HighWay.o river.o menuGenerator.o
	gcc home.o frog.o HighWay.o river.o menuGenerator.o -o home -lncurses -lpthread
home.o: home.c
	gcc -c home.c -lncurses -lpthread
HighWay.o: HighWay.c
	gcc -c HighWay.c -lncurses -lpthread
frog.o: frog.c
	gcc -c frog.c -lncurses -lpthread
river.o: river.c
	gcc -c river.c -lncurses -lpthread
menuGenerator.o: menuGenerator.c
	gcc -c menuGenerator.c -lncurses
clean:
	rm *.o
	