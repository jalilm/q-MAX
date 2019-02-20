#ifndef HEAPKV_HPP
#define HEAPKV_HPP

#include <vector>
#include <climits>
#include <utility>

typedef unsigned long long key;
typedef double val;

using std::vector;
using std::pair;

class HeapKV {
 private:
  unsigned int _size;
  unsigned int max_size;
  pair<key, val> minimal_item;
  vector<pair<key, val>> qMinHeap;
 public:
  explicit HeapKV(unsigned int _q) : max_size(_q), _size(0), minimal_item(pair<key, val>(ULLONG_MAX, ULLONG_MAX)) {}

  ~HeapKV() = default;

  bool add(pair<key, val> item);
  bool insert(key k, val v) {return add(pair<key,val>(k,v));};
  key getMinimalKey() { return minimal_item.first;};
  val getMinimalVal() { return minimal_item.second;};
  void update(key k, val v);
  vector<pair<key, val>> getItems() const;
};

#endif //HEAPKV_HPP

