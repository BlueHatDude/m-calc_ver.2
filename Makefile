app: mcalc2.o
	cc -o mcalc2 mcalc2.o main.c
	rm *.o

mcalc2.o:
	cc -c mcalc2.c -Wall -Werror -Wextra -pedantic