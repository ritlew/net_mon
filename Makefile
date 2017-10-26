make:
	gcc net_mon.c mon.c -o net_mon -lledmat

run:
	sudo ./net_mon

lib:
	gcc -shared -o ledmat.so ledmat.c -lwiringPi -lpthread
	ar -cvq ledmat.a ledmat.so

install_lib:
	sudo mv ledmat.a /usr/local/lib/libledmat.a

clean:
	rm -f ledmat.a ledmat.so