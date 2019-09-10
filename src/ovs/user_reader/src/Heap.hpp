#ifndef HEAP_HPP
#define HEAP_HPP

#include <vector>
#include <climits>

using std::vector;

class Heap {
 private:
  unsigned int _size;
  unsigned int max_size;
  int minimal_item;
  vector<int> qMinHeap;
 public:
  explicit Heap(unsigned int _q) : max_size(_q), _size(0), minimal_item(INT_MAX) {}

  ~Heap() = default;

  bool add(int item);

  vector<int> getItems() const;

  void merge(const Heap &rhs);

  void clear(int q);
};

#endif //HEAP_HPP

