CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17

# 만들 실행파일들
TARGETS = client sensor_sim

all: $(TARGETS)

client: client.o
	$(CXX) $(CXXFLAGS) -o $@ $^

sensor_sim: sensor_sim.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# 공통 규칙: .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGETS)