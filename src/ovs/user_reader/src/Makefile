all: graph34 graph34kv graph7 graph8 graph6 graph5
	
graph34: ./test/graph34.cpp ./test/Utils.hpp *.cpp *.hpp
	g++ *.cpp ./test/graph34.cpp -std=c++11 -o graph34

graph34kv: ./test/graph34kv.cpp ./test/Utils.hpp *.cpp *.hpp
	g++ *.cpp ./test/graph34kv.cpp -std=c++11 -o graph34kv

graph7: ./test/graph7.cpp ./test/Utils.hpp *.cpp *.hpp
	g++ *.cpp ./test/graph7.cpp -std=c++11 -o graph7

graph8: ./test/graph8.cpp ./test/Utils.hpp *.cpp *.hpp
	g++ *.cpp ./test/graph8.cpp -std=c++11 -o graph8

graph6: ./test/graph6.cpp ./test/Utils.hpp *.cpp *.hpp xxhash.o
	g++ xxhash.o *.cpp ./test/graph6.cpp -std=c++11 -o graph6

xxhash: ./xxhash.hpp ./xxhash.cpp
	g++ -c ./xxhash.hpp ./xxhash.cpp -o xxhash.o

graph5: ./test/graph5.cpp ./test/Utils.hpp *.cpp *.hpp
	g++ *.cpp ./test/graph5.cpp -std=c++11 -o graph5

clean:
	rm graph34 graph34kv graph5 graph6 graph7 graph8 xxhash.o
