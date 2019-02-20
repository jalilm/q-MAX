#ifndef _SkiplistIn_HPP_
#define _SkiplistIn_HPP_

#include <vector>
#include <random>
#include <utility>
#include <unordered_map>
#include <functional>

using namespace std;

template<typename key, typename val>
class SkiplistIn {
 private:
  class Node {
   public:
    bool init;
    pair<key, val> item;
    Node **next;

    explicit Node(int levels) {
      init = false;
      next = new Node *[MAX_L];
    }
    ~Node() {
      delete next;
    }
  };

  const static int MAX_L = 16;
  unsigned int _size;
  int level;
  int max_size;
  Node *head;
  unordered_map<key, Node*> hash_table;
  Node *create_node_from_pair(int level, pair<key, val> item) {
    auto *p = new Node(level);
    p->item = item;
    return p;
  }
  int random_level() {
    int level = 1;
    while (rand() % 2 && level < MAX_L) {
      level++;
    }
    return level;
  }
  bool _insert(pair<key, val> item) {
    Node *update[MAX_L];
    Node *p = head;
    Node *q = nullptr;

    for (int i = level - 1; i >= 0; --i) {
      q = p->next[i];
      while ((q != nullptr) && (q->item.second < item.second)) {
        p = q;
        q = p->next[i];
      }
      update[i] = p;
    }

    if ((q != nullptr) && (q->item.first == item.first) && (q->item.second == item.second)) {
      return false;
    }

    int rand_level = random_level();
    if (rand_level > level) {
      for (int i = level; i < rand_level; ++i) {
        update[i] = head;
      }
      level = rand_level;
    }

    q = create_node_from_pair(rand_level, item);
    if (!q) {
      return false;
    }
    hash_table[item.first] = q;
    for (int i = rand_level - 1; i >= 0; --i) {
      q->next[i] = update[i]->next[i];
      update[i]->next[i] = q;
    }
    ++_size;
    return true;
  }
  bool _remove(pair<key, val> item) {
    Node *update[MAX_L];
    Node *p = head;
    Node *q = nullptr;
    for (int i = level - 1; i >= 0; --i) {
      q = p->next[i];
      while ((q != nullptr) && (q->item.second < item.second)) {
        p = q;
        q = p->next[i];
      }
      update[i] = p;
    }
    if ((q == nullptr) || !((q->item.second == item.second) && (q->item.first == item.first))) {
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
    hash_table.erase(item.first);
    --_size;
    return true;
  }

 public:
  explicit SkiplistIn(int _q, key default_key=0, val default_val=0, bool useRandomSeed = true, unsigned int seed = 2711) : _size(0), level(0), max_size(_q) {
    head = create_node_from_pair(MAX_L - 1, pair<key, val>(default_key, default_val));
    for (int i = 0; i < MAX_L; ++i) {
      head->next[i] = nullptr;
    }
    if (useRandomSeed) {
      srand(time(0));
    } else {
      srand(seed);
    }
  }
  ~SkiplistIn() {
    Node *q = head;
    while (q != nullptr) {
      Node *next = q->next[0];
      delete (q);
      q = next;
    }
  }
  bool isIn(key k) const {
    return hash_table.count(k);
  }
  bool update(key k, val new_val, function<val(val,val)> f = [](val v1, val v2)->val{return v1+=v2;}) {
    pair<key,val> old_pair = hash_table[k]->item;
    if(_remove(old_pair)){
      pair<key,val> new_pair(k, f(old_pair.second, new_val));
      if(_insert(new_pair)) {
        return true;
      }
    }
    return false;
  }

  inline bool insert(key k, val v) {
	return add(pair<key,val>(k,v));
  }

  bool add(pair<key, val> item) {
    if (_size >= max_size) {
      auto min_item = getMinimalItem();
      if (item.second < min_item.second) {
        return false;
      } else {
        return _insert(item) && _remove(min_item);
      }
    } else {
      return _insert(item);
    }
  }
  inline unsigned long size() const {
    return _size;
  }
  inline pair<key,val> getMinimalItem() const {
    if(_size != 0) {
	return head->next[0]->item;
    }
    return head->item;
  }
  inline val getMinimalVal() const {
    return getMinimalItem().second;
  }
  vector<pair<key,val>> getItems() const {
    vector<pair<key,val>> res;
    Node *q = head->next[0];
    while (q != nullptr) {
      res.push_back(q->item);
      q = q->next[0];
    }
    return res;
  }
};

#endif

