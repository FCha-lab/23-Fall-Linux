server_launch.out: server.o
	gcc -o server_launch.out server.o -lpthread
server.o: server.c
	gcc -c server.c
client_launch1.out: client1.o
	gcc -o client_launch1.out client1.o -lpthread
client1.o: client1.c
	gcc -c client1.c
client_launch2.out: client2.o
	gcc -o client_launch2.out client2.o -lpthread
client2.o: client2.c
	gcc -c client2.c


clean:
	rm *.o

clean_launch:
	rm *.out
