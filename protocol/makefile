.PHONY:all
all:client server
client:client.cpp
	@g++ -o $@ $^ -std=c++17 -ljsoncpp
server:server.cpp
	@g++ -o $@ $^ -std=c++17 -ljsoncpp
.PHONY:clean
clean:
	@rm -f client server