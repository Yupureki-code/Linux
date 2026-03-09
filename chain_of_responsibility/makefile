.PHONY:ALL
ALL:server client
server:server.cpp
	@g++ server.cpp -o server -std=c++11
client:client.cpp
	@g++ client.cpp -o client -std=c++11

.PHONY:clean
clean:
	@rm server client