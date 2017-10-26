CC = gcc
c_flags = -Wall -pedantic
libs = -lwiringPi -lpthread

all: src/net_mon.c netutil ledutil
	$(CC) src/net_mon.c mon.o ledutil.o -o net_mon $(libs)

netutil: src/netutil.c src/netutil.h
	$(CC) -c src/netutil.c $(c_flags)

ledutil: src/ledutil.h src/ledutil.c
	$(CC) -c src/ledutil.c $(c_flags)

clean:
	rm *.o net_mon