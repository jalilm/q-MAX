#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "../QmaxKV.hpp"
#include "../HeapKV.hpp"
#include "../SkiplistKV.hpp"
#include "../xxhash.h"
#include "Utils.hpp"

#include <valgrind/callgrind.h>

#define CLK_PER_SEC CLOCKS_PER_SEC
#define CAIDA16_SIZE 152197439
#define CAIDA18_SIZE 175880896
#define UNIV1_SIZE 17323447
using namespace std;

void benchmark_hhskiplist(int q, key** keys, int numKeys) {
  key *elements = *keys;
  SkiplistKV sl(q);
  for (int i = 0; i < numKeys; i++) {
    val hash = XXH64((void *) &(elements[i]), sizeof(key), 1235);
    sl.add(pair<key, val>(elements[i], hash));
  }
}

void benchmark_hhheap(int q, key** keys, int numKeys) {
  key *elements = *keys;
  HeapKV heap(q);
  for (int i = 0; i < numKeys; i++) {
    val hash = XXH64((void *) &(elements[i]), sizeof(key), 1235);
    heap.add(pair<key,val>(elements[i], hash));
  }
}

void benchmark_hhqmax(int q, double gamma, key** keys, int numKeys) {
  key *elements = *keys;
  QMaxKV qmax = QMaxKV(q, gamma);
  for (int i = 0; i < numKeys; i++) {
    val hash = XXH64((void *) &(elements[i]), sizeof(key), 1235);
    qmax.insert(elements[i], hash);
  }
}


int main() {
  int N = 150000000;
  key* keys = (key*) malloc(sizeof(key) * N);
  for (int i = 0; i<N; ++i){
    keys[i] = std::rand();
  }

  // Uninteresting program chunk
  CALLGRIND_START_INSTRUMENTATION;

  list<unsigned int> qs = {10000000};
  for (unsigned q: qs) {
    CALLGRIND_TOGGLE_COLLECT;
    benchmark_hhheap(q, &keys, N);
    benchmark_hhskiplist(q, &keys, N);
    list<double> gammas = {0.05};
    for (double g : gammas) {
      benchmark_hhqmax(q, g, &keys, N);
    }
    CALLGRIND_TOGGLE_COLLECT;
  }
  CALLGRIND_DUMP_STATS;
  CALLGRIND_STOP_INSTRUMENTATION;
  return 0;
}

