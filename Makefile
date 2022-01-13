TARGETS=rainfall_seq rainfall_pt

all: $(TARGETS)
clean:
	rm -f $(TARGETS)

rainfall_seq: rainfall_seq.cpp landscape.cpp VectorMat.cpp DataMat.cpp
	g++ -O3 -std=c++11 -Wall -o $@ $^

rainfall_pt: rainfall_pt.cpp landscape.cpp VectorMat.cpp DataMat.cpp
	g++ -O3 -std=c++17 -Wall -pthread -o $@ $^