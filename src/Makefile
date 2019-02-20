CC = g++
FLAGS = -std=c++11
DEBUG_FLAGS =
EXEC = graph34 graph5 graph6 graph7 graph8 graph9-in graph10 graph11 hitrate
REG_OBJ = Heap.o Qmax.o Skiplist.o
KV_OBJ = HeapKV.o QmaxKV.o SkiplistKV.o
PBA_OBJ = QmaxIn.o HeapIn.o SkiplistIn.o
OBJS = xxhash.o $(REG_OBJ) $(KV_OBJ) $(PBA_OBJ) SlidingQmax.o

all: $(OBJS) $(EXEC)
	
clean:
	rm $(OBJS) $(EXEC)

xxhash.o: xxhash.c xxhash.h
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.c

Heap.o: Heap.cpp Heap.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
HeapKV.o: HeapKV.cpp HeapKV.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
HeapIn.o: HeapIn.cpp HeapIn.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
Qmax.o: Qmax.cpp Qmax.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
QmaxKV.o: QmaxKV.cpp QmaxKV.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp

QmaxIn.o: QmaxIn.cpp QmaxIn.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
Skiplist.o: Skiplist.cpp Skiplist.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
SkiplistKV.o: SkiplistKV.cpp SkiplistKV.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp

SkiplistIn.o: SkiplistIn.cpp SkiplistIn.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp
	
SlidingQmax.o: SlidingQmax.cpp Qmax.hpp SlidingQmax.hpp
	$(CC) -c $(DEBUG_FLAGS) $(FLAGS) $*.cpp

graph34: test/graph34.cpp test/Utils.hpp $(REG_OBJ)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(REG_OBJ) test/$@.cpp -o $@
	
graph5: test/graph5.cpp test/Utils.hpp $(KV_OBJS)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(KV_OBJ) test/$@.cpp -o $@
 
graph6: test/graph6.cpp test/Utils.hpp  $(KV_OBJS) xxhash.o
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(KV_OBJ) xxhash.o test/$@.cpp -o $@
 
graph7: test/graph7.cpp test/Utils.hpp $(REG_OBJ) SlidingQmax.o
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(REG_OBJ) SlidingQmax.o test/$@.cpp -o $@
	
graph8: test/graph8.cpp test/Utils.hpp $(REG_OBJ) SlidingQmax.o
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(REG_OBJ) SlidingQmax.o test/$@.cpp -o $@
 
graph9-in: test/graph9-in.cpp test/Utils.hpp $(PBA_OBJ)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(PBA_OBJ) test/$@.cpp -o $@

graph10: test/graph10.cpp test/Utils.hpp $(KV_OBJS)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(KV_OBJ) test/$@.cpp -o $@

graph11: test/graph11.cpp test/Utils.hpp $(PBA_OBJS)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(PBA_OBJ) test/$@.cpp -o $@

hitrate: test/hitrate.cpp test/Utils.hpp $(PBA_OBJS)
	$(CC) $(DEBUG_FLAGS) $(FLAGS) $(PBA_OBJ) test/$@.cpp -o $@

