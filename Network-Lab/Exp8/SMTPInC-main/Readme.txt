Compile => 
    gcc server.c -o server.o -lpthread
    gcc client.c -o client.o

Run => 
    ./server.o PORT_NO
    ./client.o PORT_NO

Note => When an email is sent, then if the To address is valid (username in email is available in logincred.txt), then that named directory will be made by code. You don't need to manually create it :) 