#ifndef HEAPIN_HPP
#define HEAPIN_HPP

#include <vector>
#include <climits>
#include <utility>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

template<typename key, typename val>
class HeapIn {
 private:
  unsigned int _size;
  unsigned int max_size;
  pair<key, val> minimal_item;
  vector<pair<key, val>> qMinHeap;
  unordered_set<key> hash_table;
 public:
  explicit HeapIn(unsigned int _q, key minimal_key=0, val minimal_val=0) : max_size(_q), _size(0), minimal_item(pair<key, val>(minimal_key, minimal_val)) {}
  ~HeapIn() = default;
  bool insert(key k, val v) {return add(pair<key,val>(k,v));};
  key getMinimalKey() { return minimal_item.first;};
  val getMinimalVal() { return minimal_item.second;};
  vector<pair<key, val>> getItems() const {
    return qMinHeap;
  }
  struct minHeapComparator {
    /**
     * Checks if i1 is "bigger" than i2 to transform heap from maxHeap to minHeap.
     * @return true if and only if i1 > i2.
     */
    bool operator()(pair<key,val> &i1, pair<key, val> &i2) {
      return i1 > i2;
    }
  };
  bool add(pair<key, val> item) {
    if (_size >= max_size) {
      if (!minHeapComparator()(item, minimal_item)) { // DIRECT COMPARSON: check if new val is less than minimal val
        return false;
      } else {
        pop_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
        qMinHeap.pop_back();
        hash_table.erase(minimal_item.first);
        qMinHeap.push_back(item);
        hash_table.insert(item.first);
        push_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
        minimal_item = qMinHeap.front();
        return true;
      }
    } else {
      qMinHeap.push_back(item);
      hash_table.insert(item.first);
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
  bool isIn(key k) {
    return hash_table.count(k);
  }
  void update(key k, val v, std::function<void(val&,val&)> f = [](val& v1, val& v2) {v1+=v2;}) {
    for (auto & it: qMinHeap) {
	if(it.first == k) {
		val& old_val = it.second;
		f(old_val, v);
		make_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
		return;
	}
    }
  }
};

#endif

