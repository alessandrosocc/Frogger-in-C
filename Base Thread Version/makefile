home: home.o frog.o HighWay.o
	gcc home.o frog.o HighWay.o -o home -lncurses -lpthread
home.o: home.c
	gcc -c home.c -lncurses -lpthread
HighWay.o: HighWay.c
	gcc -c HighWay.c -lncurses -lpthread
frog.o: frog.c
	gcc -c frog.c -lncurses -lpthread
clean:
	rm *.o
	