#include "HeapKV.hpp"
#include <algorithm>
#include <utility>
#include <functional>

using std::pop_heap;
using std::push_heap;
using std::string;
using std::pair;

struct minHeapComparator {
  /**
   * Checks if i1 is "bigger" than i2 to transform heap from maxHeap to minHeap.
   * @return true if and only if i1 > i2.
   */
  bool operator()(pair<key,val> &i1, pair<key, val> &i2) {
    return i1 > i2;
  }
};

bool HeapKV::add(pair<key, val> item) {
  if (_size >= max_size) {
     if (!minHeapComparator()(item, minimal_item)) { // DIRECT COMPARSON: check if new val is less than minimal val
      return false;
    } else {
      pop_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
      qMinHeap.pop_back();
      qMinHeap.push_back(item);
      push_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
      minimal_item = qMinHeap.front();
      return true;
    }
  } else {
    qMinHeap.push_back(item);
    _size++;
    if(_size != 1) {
        if(!minHeapComparator()(item, minimal_item)) { // DIRECT COMPARSION: update minimal item
            minimal_item = item;
        }
    } else {
	minimal_item = qMinHeap.front();
    }
    if(_size == max_size) {
      make_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
    }
    return true;
  }
}

inline vector<pair<key, val>> HeapKV::getItems() const {
  return qMinHeap;
}

void HeapKV::update(key k, val v) {
	for (auto& it: qMinHeap) {
		if (it.first == k) {
			it.second = v;
		}
	}
}

