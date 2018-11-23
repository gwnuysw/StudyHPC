all: server client

server: server.c smemory.h
	cc -o server server.c

client: client.c smemory.h
	cc -o client client.c

clean:
	rm -f *.o
