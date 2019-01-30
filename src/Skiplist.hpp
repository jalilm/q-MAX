#ifndef _Skiplist_H_
#define _Skiplist_H_

#include <vector>
#include <random>

using std::vector;

class Skiplist {
 private:
  class Node {
   public:
    bool init;
    int item;
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
  Node *create_node_from_int(int level, int item);
  int random_level();
  bool _insert(int item);
  bool _remove(int item);

 public:
  explicit Skiplist(int _q, bool useRandomSeed = true, unsigned int seed = 2711) : _size(0), level(0), max_size(_q) {
    head = create_node_from_int(MAX_L - 1, -1);
    for (int i = 0; i < MAX_L; ++i) {
      head->next[i] = nullptr;
    }
    if (useRandomSeed) {
      srand(time(0));
    } else {
      srand(seed);
    }
  }

  ~Skiplist() {
    Node *q = head;
    while (q != nullptr) {
      Node *next = q->next[0];
      delete (q);
      q = next;
    }
  }

  bool add(int item);
  unsigned long size() const;
  int getMinimalItem() const;
  vector<int> getItems() const;
  void merge(const vector<int> &rhs);
};

#endif

