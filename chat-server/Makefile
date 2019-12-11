CFLAGS= -Wall -pedantic -g -std=c99 

all: cserver cclient
cserver: cserver.c
	gcc $(CFLAGS) -o cserver cserver.c

cclient: cclient.c
	gcc $(CFLAGS) -o cclient cclient.c

.PHONY: clean
clean:
	rm cclient cserver
