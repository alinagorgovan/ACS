CFLAGS = -Wall -g

build: server client

server: server.c
	gcc -Wall -g server.c -o server

client: client.c
	gcc -Wall -g client.c -o client
.PHONY: clean run_	server run_client

run_server:
	./server

run_client:
	./client

clean:
	rm -f server client
clean_logs:
	rm client-*
