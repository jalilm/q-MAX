#include "heapKV.hpp"
#include <algorithm>
#include <utility>
#include <string>

using std::pop_heap;
using std::push_heap;
using std::string;
using std::pair;

struct minHeapComparator {
  /**
   * Checks if i1 is "bigger" than i2.
   * @return true if and only if i1 > i2.
   */
  bool operator()(pair<key,val> &i1, pair<key, val> &i2) {
    return i1 > i2;
  }
};

bool heapKV::add(pair<key, val> item) {
  if (_size >= max_size) {
    if (item <= minimal_item) {
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
    if(item < minimal_item) {
      minimal_item = item;
    }
    if(_size == max_size) {
      make_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
    }
    return true;
  }
}

inline vector<pair<key, val>> heapKV::getItems() const {
  return qMinHeap;
}
