#ifndef HEAPPBAKV_HPP
#define HEAPPBAKV_HPP

#include <vector>
#include <climits>
#include <utility>
#include <unordered_set>

typedef unsigned long long weight;

typedef struct h_pba_val {
	public:
        weight w;
        double a;
        double q;
        double u;
        double w_by_u;
} h_pba_val;

typedef unsigned long long key;
typedef h_pba_val h_val;

using std::pair;
using std::vector;
using std::unordered_map;

class HeapPBAKV {
 private:
  unordered_set<key> hash_table;
  unsigned int _size;
  unsigned int max_size;
  pair<key, h_val> minimal_item;
  vector<pair<key, h_val>> qMinHeap;
  weight z_star;
 public:
  explicit HeapPBAKV(unsigned int _q) : max_size(_q), _size(0), minimal_item(pair<key, h_val>(ULLONG_MAX, {1,1,1,1,0})), z_star(0) {}

  ~HeapPBAKV() = default;

  bool add(key k, weight x, double u);

  vector<pair<key, h_val>> getItems() const;
};

#endif //HEAPPBAKV_HPP

