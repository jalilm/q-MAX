all: user_reader

USER_HEADR = tuple.h ringbuffer.h util.h
USER_SRC = ringbuffer_user.cpp src/Heap.cpp src/Skiplist.cpp src/Qmax.cpp src/HeapKV.cpp src/SkiplistKV.cpp src/QmaxKV.cpp src/xxhash.c
USER_LIB = -lpthread -lrt
INS_SET = -mbmi -msse -msse3 -msse4.1 -msse4.2 -mavx -march=native

user_reader: user_reader.cpp $(USER_HEADER) $(USER_SRC)
	g++ -g -Wall -std=c++1z -o $@ $< $(USER_SRC) $(USER_LIB) $(INS_SET)

user_reader_ps: user_reader_ps.cpp $(USER_HEADER) $(USER_SRC)
	g++ -g -Wall -std=c++1z -o $@ $< $(USER_SRC) $(USER_LIB) $(INS_SET)

user_reader_hh: user_reader_hh.cpp $(USER_HEADER) $(USER_SRC)
	g++ -g -Wall -std=c++1z -o $@ $< $(USER_SRC) $(USER_LIB) $(INS_SET)

clean:
	rm -rf user_reader user_reader_ps user_reader_hh
