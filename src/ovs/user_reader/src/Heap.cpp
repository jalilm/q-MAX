#include "Heap.hpp"
#include <algorithm>
#include <string>

using std::pop_heap;
using std::push_heap;
using std::string;

struct minHeapComparator {
  /**
   * Checks if i1 is "bigger" than i2.
   * @return true if and only if i1 > i2.
   */
  bool operator()(int &i1,int &i2) {
    return i1 > i2;
  }
};

bool Heap::add(int item) {
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

inline vector<int> Heap::getItems() const {
  return qMinHeap;
}

void Heap::merge(const Heap &rhs) {
  const auto v2 = rhs.getItems();
  qMinHeap.insert(qMinHeap.end(), v2.begin(), v2.end());
  sort(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
  if (qMinHeap.size() > max_size) {
    qMinHeap.erase(qMinHeap.begin() + max_size, qMinHeap.end());
  }
  make_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
}

void Heap::clear(int q) {
  qMinHeap.clear();
  _size = 0;
  max_size = q;
  minimal_item = INT_MAX;
}
