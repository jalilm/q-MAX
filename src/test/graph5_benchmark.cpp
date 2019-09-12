#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <random>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include <time.h>
#include <stdlib.h>

#include <valgrind/callgrind.h>

#include "../QmaxKV.hpp"
#include "../HeapKV.hpp"
#include "../SkiplistKV.hpp"
#include "Utils.hpp"

#define CLK_PER_SEC CLOCKS_PER_SEC
#define CAIDA16_SIZE 152197437
#define CAIDA18_SIZE 175880808
#define UNIV1_SIZE 17323447

using namespace std;

void benchmark_psskiplist(int q, key** keys, val** vals, int numKeys) {
  srand(12345);
  double r = ((double)rand()) / ((double)RAND_MAX);
  key *elements = *keys;
  val *weights = *vals;
  SkiplistKV sl(q);
  for (int i = 0; i < numKeys; i++) {
    val priority = weights[i] / (1-r);
    sl.add(pair<key, val>(elements[i], priority));
  }
  sl.getItems();
}

void benchmark_psheap(int q, key** keys, val** vals, int numKeys) {
  srand(12345);
  double r = ((double)rand()) / ((double)RAND_MAX);
  key *elements = *keys;
  val *weights = *vals;
  HeapKV heap(q);
  for (int i = 0; i < numKeys; i++) {
    val priority = weights[i] / (1-r);
    heap.add(pair<key,val>(elements[i], priority));
  }
  heap.getItems();
}

void benchmark_psqmax(int q, double gamma, key** keys, val** vals, int numKeys) {
  srand(12345);
  double r = ((double)rand()) / ((double)RAND_MAX);
  key *elements = *keys;
  val *weights = *vals;
  QMaxKV qmax = QMaxKV(q, gamma);
  for (int i = 0; i < numKeys; i++) {
    val priority = weights[i] / (1-r);
    qmax.insert(elements[i], priority);
  }
  qmax.largestQ();
}

int main() {
  int N = 150000000;
  key* keys = (key*) malloc(sizeof(key) * N);
  val* vals = (val*) malloc(sizeof(val) * N);
  for (int i = 0; i<N; ++i){
    keys[i] = std::rand();
    vals[i] = std::rand();
  }

  // Uninteresting program chunk
  CALLGRIND_START_INSTRUMENTATION;

  list<unsigned int> qs = {10000};
  for (unsigned q: qs) {
    CALLGRIND_TOGGLE_COLLECT;
    benchmark_psheap(q, &keys, &vals, N);
    benchmark_psskiplist(q, &keys, &vals, N);
    list<double> gammas = {0.05};
    for (double g : gammas) {
      benchmark_psqmax(q, g, &keys, &vals, N);
    }
    CALLGRIND_TOGGLE_COLLECT;
  }
  CALLGRIND_DUMP_STATS;
  CALLGRIND_STOP_INSTRUMENTATION;
  return 0;
}

