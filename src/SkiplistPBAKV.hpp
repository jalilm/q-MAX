#ifndef _SkiplistKV_H_
#define _SkiplistKV_H_

#include <vector>
#include <random>
#include <utility>
#include <unordered_map>

using namespace std;

typedef unsigned long long weight;

typedef struct sl_pba_val {
        weight w;
        double a;
        double q;
        double u;
        double w_by_u;
} sl_pba_val;

typedef unsigned long long key;
typedef sl_pba_val sl_val;


class SkiplistKV {
 private:
  class Node {
   public:
    bool init;
    pair<key, sl_val> item;
    Node **next;

    explicit Node(int levels) {
      init = false;
      next = new Node *[MAX_L];
    }
    ~Node() {
      delete next;
    }
  };

  unordered_map<key, sl_val*> hash_table;

  const static int MAX_L = 16;
  unsigned int _size;
  int level;
  int max_size;
  Node *head;
  Node *create_node_from_int(int level, pair<key, sl_val> item);
  int random_level();
  bool _insert(pair<key, sl_val> item);
  bool _remove(pair<key, sl_val> item);

 public:
  explicit SkiplistKV(int _q, bool useRandomSeed = true, unsigned int seed = 2711) : _size(0), level(0), max_size(_q) {
    head = create_node_from_int(MAX_L - 1, pair<key, sl_val>({-1, {0, 0, 0, 0, -1}}));
    for (int i = 0; i < MAX_L; ++i) {
      head->next[i] = nullptr;
    }
    if (useRandomSeed) {
      srand(time(0));
    } else {
      srand(seed);
    }
  }

  ~SkiplistKV() {
    Node *q = head;
    while (q != nullptr) {
      Node *next = q->next[0];
      delete (q);
      q = next;
    }
  }

  bool add(pair<key, sl_val> item);
  unsigned long size() const;
  pair<key,sl_val> getMinimalItem() const;
  vector<pair<key, sl_val>> getItems() const;
};

#endif

