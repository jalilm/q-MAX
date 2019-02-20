#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "../Qmax.hpp"
#include "../Heap.hpp"
#include "../Skiplist.hpp"
#include "Utils.hpp"

#define N 150000000
#define CLK_PER_SEC CLOCKS_PER_SEC

using namespace std;

void benchmark_skiplist(int q, int** data, ofstream &ostream) {
  int *elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  Skiplist sl(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
         sl.add(elements[i]);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,SkipList," << N << "," << q << ",," << time << endl;
}

void benchmark_heap(int q, int** data, ofstream &ostream) {
  int *elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  Heap heap(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    heap.add(elements[i]);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,Heap," << N << "," << q << ",," << time << endl;
}

void benchmark_qmax(int q, double gamma, int** data, ofstream &ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMax qmax(q, gamma);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    qmax.insert(elements[i]);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,AmortizedQMax," << N << "," << q << "," << gamma << "," << time << endl;
}

int main() {
  ofstream ostream;
  setupOutputFile("../results/timing_random.raw_res", ostream, false);
  for (int run = 0; run < 5; run++) {
    int* data = (int*) malloc(sizeof(int) * N);
    for (int i = 0; i< N; ++i){
      data[i] = std::rand();
    }
    list<int> qs = {10000, 100000, 1000000, 10000000};
    for (int q : qs) {
      benchmark_heap(q, &data, ostream);
      benchmark_skiplist(q, &data, ostream);
      list<double> gammas = {0.025};
      for (double g : gammas) {
        benchmark_qmax(q, g, &data, ostream);
      }
    }
  }
  ostream.close();
  return 0;
}

