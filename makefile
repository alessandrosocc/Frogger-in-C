home: home.o HighWay.o
	gcc home.o HighWay.o -o home -lncurses
HighWay.o: HighWay.c HighWay.h
	gcc -c HighWay.c -lncurses
clean:
	rm *.o
