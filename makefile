CC:=gcc
CFLAGS:=-O3 -c
FILE:=mcx

all: mcx

mcx:
	$(CC) $(CFLAGS) $(FILE).c -o $(FILE).o

clean:
	rm -rf $(FILE).o
