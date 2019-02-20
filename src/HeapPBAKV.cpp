#include "HeapPBAKV.hpp"
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <iostream>

struct minHeapComparator {
  /**
   * Checks if i1 is "bigger" than i2 to transform heap from maxHeap to minHeap.
   * @return true if and only if i1 > i2.
   */
  bool operator()(pair<key,h_val> &i1, pair<key,h_val> &i2) {
    return i1.second.w_by_u > i2.second.w_by_u;
  }
};

using namespace std;

bool HeapPBAKV::add(key k, weight x, double u) {
  cout << "before find" << endl;
  auto it = hash_table.find(k);
  cout << "after find" << endl;
  if(it != hash_table.end()) {
//  for (auto& p : qMinHeap) {
//	if(p.first == k) {
		cout << "in ";
		auto &v = it->second->second;
		v.a *= v.q;
		if (z_star != 0) {
	                double w_by_z_star = v.w / z_star;
	                v.q = v.q < (w_by_z_star) ? v.q : w_by_z_star;
		}
		if(v.q != 0) {
	                v.a /= v.q;
		}
                v.a += x;
                v.w += x;
                v.w_by_u = v.w / v.u;
                return true;
	}
//  }
  h_val v = {x, x, 1, u, x/u};
  pair<key,h_val> item(k, v);
  if (_size >= max_size) {
    if (!minHeapComparator()(item, minimal_item)) { // DIRECT COMPARSON: check if new h_val is less than minimal h_val
       z_star = (z_star > v.w_by_u) ? z_star : v.w_by_u;
       return false;
    } else {
      pop_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
      qMinHeap.pop_back();
      qMinHeap.push_back(item);
      hash_table[k] = &(qMinHeap.back());
      push_heap(qMinHeap.begin(), qMinHeap.end(), minHeapComparator());
      weight w_by_u_star = minimal_item.second.w_by_u;
      z_star = (z_star > w_by_u_star) ? z_star : w_by_u_star;
      minimal_item = qMinHeap.front();
      hash_table.erase(hash_table.find(minimal_item.first));
      return true;
    }
  } else {
    cout <<"out 2";
    qMinHeap.push_back(item);
    hash_table[k] = &(qMinHeap.back());
    cout << "afer out 2 insertion";
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
    cout << "before return";
    return true;
  }
}

inline vector<pair<key, h_val>> HeapPBAKV::getItems() const {
  return qMinHeap;
}

