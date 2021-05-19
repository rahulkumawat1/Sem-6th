compile:
    gcc server.c -o server.o -l pthread 

run (server):
    ./server.o

browser (client):
    127.0.0.1/8080/  ==> home page
    127.0.0.1/8080/mypage.html  ==> mypage.html file in this folder
    127.0.0.1/8080/anything  ==> 404 error