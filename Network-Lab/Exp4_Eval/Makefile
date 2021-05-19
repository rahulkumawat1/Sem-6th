compile:
	gcc -Wall -g3 -fsanitize=address -pthread server.c -o server
	gcc -Wall -g3 -fsanitize=address -pthread client.c -o client

run:
	./server PORT_NUM
	./client PORT_NUM
