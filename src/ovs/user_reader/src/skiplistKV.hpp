#ifndef _SkiplistKV_H_
#define _SkiplistKV_H_

#include <vector>
#include <random>
#include <utility>

using std::vector;
using std::pair;

typedef unsigned long long key;
typedef unsigned long long val;

class skiplistKV {
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
  Node *create_node_from_int(int level, pair<key, val> item);
  int random_level();
  bool _insert(pair<key, val> item);
  bool _remove(pair<key, val> item);

 public:
  explicit skiplistKV(int _q, bool useRandomSeed = true, unsigned int seed = 2711) : _size(0), level(0), max_size(_q) {
    head = create_node_from_int(MAX_L - 1, pair<key, val>(-1, -1));
    for (int i = 0; i < MAX_L; ++i) {
      head->next[i] = nullptr;
    }
    if (useRandomSeed) {
      srand(time(0));
    } else {
      srand(seed);
    }
  }

  ~skiplistKV() {
    Node *q = head;
    while (q != nullptr) {
      Node *next = q->next[0];
      delete (q);
      q = next;
    }
  }

  bool add(pair<key, val> item);
  unsigned long size() const;
  pair<key,val> getMinimalItem() const;
  vector<pair<key, val>> getItems() const;
};

#endif
