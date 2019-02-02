CC=gcc
CFLAGS=-pedantic -Wall -ansi
LINKFLAGS=-lm
all: simplecalc.o graficador.o supercalc.o bignum.o listas.o main.o
	$(CC) $(CFLAGS) -o main.exe *.o $(LINKFLAGS)
bignum.o:
	$(CC) $(CFLAGS) -c -o bignum.o bignum.c $(LINKFLAGS)
graficador.o:
	$(CC) $(CFLAGS) -c -o graficador.o graficador.c $(LINKFLAGS)
simplecalc.o:
	$(CC) $(CFLAGS) -c -o simplecalc.o simplecalc.c $(LINKFLAGS)
supercalc.o:
	$(CC) $(CFLAGS) -c -o supercalc.o supercalc.c $(LINKFLAGS)
listas.o:
	$(CC) $(CFLAGS) -c -o listas.o listas.c $(LINKFLAGS)
main.o:
	$(CC) $(CFLAGS) -c -o main.o main.c $(LINKFLAGS)
clean:
	rm *.o *.exe
