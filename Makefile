main: client server

client_thread: client_thread.c
	cc client_thread.c -o client_thread -g -lpthread -lm

client: client.c
	cc client.c -o client

server: server.c
	cc server.c -o server

clean:
	rm -rf client server