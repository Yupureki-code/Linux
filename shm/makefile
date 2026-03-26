.PHONY:all
all:client server
client:client.cpp
	@g++ -o $@ $^ -g
server:server.cpp
	@g++ -o $@ $^ -g
.PHONY:clean
clean:
	@rm -f client server