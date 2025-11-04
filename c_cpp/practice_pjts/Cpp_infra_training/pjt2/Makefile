CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11

all: server client

server: server.o
	$(CXX) $(CXXFLAGS) -o $@ $^

client: client.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o server client
