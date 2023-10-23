CC=gcc


app: mcalc2.o main.c
	$(CC) -o mcalc2 mcalc2.o main.c


mcalc2.o: mcalc2.c
	cc -c mcalc2.c -Wall -Werror -Wextra -pedantic

