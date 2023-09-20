CC=gcc

app: mcalc2.o
	$(CC) -o mcalc2 mcalc2.o main.c


mcalc2.o:
	cc -c mcalc2.c -Wall -Werror -Wextra -pedantic

