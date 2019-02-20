#include "SkiplistPBAKV.hpp"
#include <stdexcept>
#include <utility>
#include <unordered_map>

struct minSkiplistComparator {
  /**
   * Checks if i1 is "bigger" than i2 to transform heap from maxHeap to minHeap.
   * @return true if and only if i1 > i2.
   */
  bool operator()(pair<key,sl_val> &i1, pair<key,sl_val> &i2) {
    return i1.second.w_by_u > i2.second.w_by_u;
  }
};

struct minSkiplistEqual {
  /**
   * Checks if i1 is "bigger" than i2 to transform heap from maxHeap to minHeap.
   * @return true if and only if i1 > i2.
   */
  bool operator()(pair<key,sl_val> &i1, pair<key,sl_val> &i2) {
    return i1.second.w_by_u == i2.second.w_by_u && i1.first == i2.first;
  }
};

minSkiplistComparator sl_comp = minSkiplistComparator();
minSkiplistEqual sl_eq = minSkiplistEqual();

SkiplistKV::Node *SkiplistKV::create_node_from_int(int level, pair<key, sl_val> item) {
  auto *p = new SkiplistKV::Node(level);
  p->item = item;
  return p;
}

int SkiplistKV::random_level() {
  int level = 1;
  while (rand() % 2 && level < MAX_L) {
    level++;
  }
  return level;
}

bool SkiplistKV::add(pair<key,sl_val> item) {
  auto it = hash_table.find(k);
  if(it != hash_table.end()) {
                auto v = it->second;
                v->a *= v->q;
                if (z_star != 0) {
                        double w_by_z_star = v->w / z_star;
                        v->q = v->q < (w_by_z_star) ? v->q : w_by_z_star;
                }
                if(v->q != 0) {
                        v->a /= v->q;
                }
                v->a += x;
                v->w += x;
                v->w_by_u = v->w / v->u;
                return true;
  }

  sl_val v = {x, x, 1, u, x/u};
  pair<key,sl_val> item(k, v);

  bool added = false;
  if (_size >= max_size) {
    auto min_item = getMinimalItem();
    if (!sl_comp(item,min_item)) {
      return false;
    } else {
/*    
      bool added = _insert(item);
      if (added) {
        _remove(min_item);
      }
      return added;
*/
      _insert(item);
      _remove(min_item);
    }
  } else {
    hash_table[k] = &(qMinHeap.back().second);
    return _insert(item);
  }
}

bool SkiplistKV::_insert(pair<key,sl_val> item, ) {
  Node *update[MAX_L];
  Node *p = head;
  Node *q = nullptr;

  for (int i = level - 1; i >= 0; --i) {
    q = p->next[i];
    while ((q != nullptr) && sl_comp(q->item, item)) {
      p = q;
      q = p->next[i];
    }
    update[i] = p;
  }

  if ((q != nullptr) && sl_eq(p->item,item)) {
    // DIRECT COMPARSION: we found the exact k,v pair. what should we do? it is application defined.
    return false;
  }

  int rand_level = random_level();
  if (rand_level > level) {
    for (int i = level; i < rand_level; ++i) {
      update[i] = head;
    }
    level = rand_level;
  }

  q = create_node_from_int(rand_level, item);
  if (!q) {
    return false;
  }

  for (int i = rand_level - 1; i >= 0; --i) {
    q->next[i] = update[i]->next[i];
    update[i]->next[i] = q;
  }
  ++_size;
  return true;
}

bool SkiplistKV::_remove(pair<key,sl_val> item) {
  Node *update[MAX_L];
  Node *p = head;
  Node *q = nullptr;
  for (int i = level - 1; i >= 0; --i) {
    q = p->next[i];
    while ((q != nullptr) && sl_comp(q->item, item)) { 
      p = q;
      q = p->next[i];
    }
    update[i] = p;
  }
  if ((q == nullptr) || !sl_eq(q->item,item)) {
    return false;
  }

  for (int i = level - 1; i >= 0; --i) {
    if (update[i]->next[i] == q) {
      update[i]->next[i] = q->next[i];
      if (head->next[i] == nullptr) {
        level--;
      }
    }
  }
  delete (q);
  --_size;
  return true;
}


inline unsigned long SkiplistKV::size() const {
  return _size;
}

inline pair<key,sl_val> SkiplistKV::getMinimalItem() const {
  if (_size != 0) {
    return head->next[0]->item;
  } else {
    throw std::runtime_error("can not get minimal item when SkiplistKV is empty.");
  }
}

vector<pair<key,sl_val>> SkiplistKV::getItems() const {
  vector<pair<key,sl_val>> res;
  Node *q = head->next[0];
  while (q != nullptr) {
    res.push_back(q->item);
    q = q->next[0];
  }
  return res;
}

