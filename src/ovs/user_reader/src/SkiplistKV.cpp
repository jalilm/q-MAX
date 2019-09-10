#include "SkiplistKV.hpp"
#include <stdexcept>
#include <utility>

SkiplistKV::Node *SkiplistKV::create_node_from_int(int level, pair<key, val> item) {
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

bool SkiplistKV::add(pair<key,val> item) {
  bool added = false;
  if (_size >= max_size) {
    auto min_item = getMinimalItem();
    if (item <= min_item) {
      return false;
    } else {
      bool added = _insert(item);
      if (added) {
        _remove(min_item);
      }
      return added;
    }
  } else {
    return _insert(item);
  }
}

bool SkiplistKV::_insert(pair<key,val> item) {
  Node *update[MAX_L];
  Node *p = head;
  Node *q = nullptr;

  for (int i = level - 1; i >= 0; --i) {
    q = p->next[i];
    while ((q != nullptr) && (q->item < item)) {
      p = q;
      q = p->next[i];
    }
    update[i] = p;
  }

  if ((q != nullptr) && (q->item == item)) {
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

bool SkiplistKV::_remove(pair<key,val> item) {
  Node *update[MAX_L];
  Node *p = head;
  Node *q = nullptr;
  for (int i = level - 1; i >= 0; --i) {
    q = p->next[i];
    while ((q != nullptr) && (q->item < item)) {
      p = q;
      q = p->next[i];
    }
    update[i] = p;
  }
  if ((q == nullptr) || (q->item != item)) {
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

inline pair<key,val> SkiplistKV::getMinimalItem() const {
  if (_size != 0) {
    return head->next[0]->item;
  } else {
    throw std::runtime_error("can not get minimal item when SkiplistKV is empty.");
  }
}

vector<pair<key,val>> SkiplistKV::getItems() const {
  vector<pair<key,val>> res;
  Node *q = head->next[0];
  while (q != nullptr) {
    res.push_back(q->item);
    q = q->next[0];
  }
  return res;
}

