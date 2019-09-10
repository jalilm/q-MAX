#ifndef HEAPKV_HPP
#define HEAPKV_HPP

#include <vector>
#include <climits>
#include <utility>

typedef unsigned long long key;
typedef unsigned long long val;

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

  vector<pair<key, val>> getItems() const;
};

#endif //HEAPKV_HPP

