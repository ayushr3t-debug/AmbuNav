CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
SRC = src/Graph.cpp src/HashTable.cpp src/LinkedList.cpp src/Vehicle.cpp \
      src/Ambulance.cpp src/RegularVehicle.cpp src/TrafficSimulator.cpp \
      src/Analytics.cpp src/main.cpp
OUT = ambunav

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

.PHONY: all clean
