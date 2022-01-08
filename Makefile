TARGETS=rain prain

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

rain: utils.cpp landscape.cpp vectormap.cpp datamap.cpp
	g++ -O3 -std=c++11 -Wall -o $@ $^

prain: utils_pt.cpp landscape.cpp vectormap.cpp datamap.cpp
	g++ -O3 -std=c++14 -Wall -pthread -o $@ $^