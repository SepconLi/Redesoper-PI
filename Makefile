server: UDPServer.c
	rm -rf ./server && rm -rf ./client && cc -g -Wall -Wextra UDPServer.c -o server -lm && cc -g -Wall -Wextra UDPClient.c -o client -lm

client: UDPClient.c
	cc -g -Wall -Wextra UDPClient.c -o client -lm

.PHONY: s
s:
	clear && ./server

.PHONY: c
c:
	clear && ./client

.PHONY: clean
clean:
	rm -rf ./server && rm -rf ./client
