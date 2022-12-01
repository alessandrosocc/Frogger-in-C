home: home.o HighWay.o
	gcc -lncurses home.o HighWay.o -o home
HighWay.o: HighWay.c HighWay.h
	gcc -lncurses -c HighWay.c
clean:
	rm *.o