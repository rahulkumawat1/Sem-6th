# DNS_Server

A Local dns server that finds A and AAAA CNAME and NS by listening to port and uses nslookup to find the value if not in cache

# usage

run the server.c
gcc server.c -lpthread -o server.o
./server.o 1234
<br>
nslookup -type=a -port=1234 nitc.ac.in 127.0.0.1
