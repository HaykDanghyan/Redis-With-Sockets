Redis: *.cpp
	g++ -o Redis -std=c++2a src/*.cpp server.cpp main.cpp

client:
	g++ -o client -std=c++2a client.cpp src/*.cpp 
clean:
	rm -f Redis

clear:
	rm -f client