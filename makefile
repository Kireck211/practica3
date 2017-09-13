ll : pi pi1 pi2 pi3 pi3_child

pi : pi.o
	gcc -o pi pi.o

pi.o : pi.c
	gcc -c pi.c

pi1 : pi1.o
	gcc -o pi1 pi1.o -l pthread

pi1.o : pi1.c
	gcc -c pi1.c -l pthread

pi2 : pi2.o
	gcc -o pi2 pi2.o

pi2.o : pi2.c
	gcc -c pi2.c

pi3 : pi3.o
	gcc -o pi3 pi3.o

pi3.o : pi3.c
	gcc -c pi3.c

pi3_child : pi3_child.o
	gcc -o pi3_child pi3_child.o

pi3_child.o : pi3_child.c
	gcc -c pi3_child.c

clean:
	rm pi.o pi1.o pi2.o pi3.o pi3_child.o pi pi1 pi2 pi3 pi3_child
