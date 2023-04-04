CC = gcc
CFLAGS = -Wall -Wextra


default: NetworkAdmin
	$(CC) $(CFLAGS) Client.c NetworkAdmin.c -o Client
	$(CC) $(CFLAGS) Server.c NetworkAdmin.c -o Server


client: NetworkAdmin
	$(CC) $(CFLAGS) Client.c NetworkAdmin.c -o Client

server: NetworkAdmin
	$(CC) $(CFLAGS) Server.c NetworkAdmin.c -o Server

NetworkAdmin:
	$(CC) $(CFLAGS) -c NetworkAdmin.c -o NetworkAdmin.o

clean:
	rm -f client server NetworkAdmin.o
