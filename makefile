ll : pi1 pi2 pi3

pi1 : pi1.o
	gcc -o pi1 pi1.o

pi1.o : pi1.c
	gcc -c pi1.c

pi2 : pi2.o
	gcc -o pi2 pi2.o

pi2.o : pi2.c
	gcc -c pi2.c

pi3 : pi3.o
	gcc -o pi3 pi3.o

pi3.o : pi3.c
	gcc -c pi3.c

clean:
	rm pi1.o pi2.o pi3.o pi1 pi2 pi3 
