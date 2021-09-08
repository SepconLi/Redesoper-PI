server: PruebaUDPServer.c
	cc -g -Wall -Wextra PruebaUDPServer.c -o server -lm && cc -g -Wall -Wextra PruebaUDPClient.c -o client -lm

client: PruebaUDPClient.c
	cc -g -Wall -Wextra PruebaUDPClient.c -o client -lm

.PHONY: s
s:
	./server

.PHONY: c
c:
	./client

.PHONY: clean
clean:
	rm -rf ./server && rm -rf ./client
