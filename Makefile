
all:
	gcc client.c ope.c -o client -lsqlite3
	gcc server.c ope.c -o server -lsqlite3 -lpthread

clean:
	rm server client
