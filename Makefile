all: server client

server:
	g++ server.cpp -o server -w

client:
	g++ client.cpp -o client -w