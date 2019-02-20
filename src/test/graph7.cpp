#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "../SlidingQmax.hpp"
#include "../Qmax.hpp"
#include "../Heap.hpp"
#include "../Skiplist.hpp"
#include "Utils.hpp"

#define N 150000000
#define STEP 1000000
#define CLK_PER_SEC CLOCKS_PER_SEC

using namespace std;

void benchmark_qmax_steps(int q, double gamma, int** data, ofstream& ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time = 0;
  QMax qmax = QMax(q, gamma);
  for (int i = 0; i < N;) {
    begint = clock();
    ftime(&begintb);
    for (int j = 0; j < STEP; j++, i++) {
      qmax.insert(elements[i]);
    }
    endt = clock();
    ftime(&endtb);
    time += ((double)(endt-begint))/CLK_PER_SEC;
    ostream << "random,AmortizedQMax," << i << "," << q << "," << gamma << "," << time << endl;
  }
}

void benchmark_heap_steps(int q, int** data, ofstream& ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time = 0;
  Heap heap = Heap(q);
  for (int i = 0; i < N;) {
    begint = clock();
    ftime(&begintb);
    for (int j = 0; j < STEP; j++, i++) {
      heap.add(elements[i]);
    }
    endt = clock();
    ftime(&endtb);
    time += ((double)(endt-begint))/CLK_PER_SEC;
    ostream << "random,Heap," << i << "," << q << ",," << time << endl;
  }
}

void benchmark_skiplist_steps(int q, int** data, ofstream& ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time = 0;
  Skiplist sl = Skiplist(q);
  for (int i = 0; i < N;) {
    begint = clock();
    ftime(&begintb);
    for (int j = 0; j < STEP; j++, i++) {
      sl.add(elements[i]);
    }
    endt = clock();
    ftime(&endtb);
    time += ((double)(endt-begint))/CLK_PER_SEC;
    ostream << "random,SkipList," << i << "," << q << ",," << time << endl;
  }
}



void benchmark_sqmax_steps(int q, double gamma, int n, int W, int** data, ofstream& ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time = 0;
  SlidingQMax sqmax = SlidingQMax(W, q, gamma, 1.0/n);
  for (int i = 0; i < N;) {
    begint = clock();
    ftime(&begintb);
    for (int j = 0; j < STEP; j++, i++) {
      sqmax.insert(elements[i]);
    }
    endt = clock();
    ftime(&endtb);
    time += ((double)(endt-begint))/CLK_PER_SEC;
    ostream << "random,Sliding Window q-max," << i << "," << q << "," << gamma << "," << W << "," << double(1.0/n) << "," << time << endl;
  }
}

int main() {
/*  ofstream sliding_stream;
  setupOutputFile("../results/stiming_steps_random.raw_res", sliding_stream, true);

  ofstream qmax_stream;
  setupOutputFile("../results/timing_steps_random.raw_res", qmax_stream, false);
*/

  ofstream heap_stream;
  setupOutputFile("../results/timing_heap_steps_random.raw_res", heap_stream, false);

  ofstream skiplist_stream;
  setupOutputFile("../results/timing_skiplist_steps_random.raw_res", skiplist_stream, false);


  for (int run = 0; run < 5; run++) {
    int* data = (int*) malloc(sizeof(int) * N);
    int* vals = (int*) malloc(sizeof(int) * N);
    for (int i = 0; i< N; ++i){
      data[i] = std::rand();
      vals[i] = std::rand();
    }
/*
    list<double> gammas = {0.1, 0.05, 0.01, 0.005, 0.001, 0.005};
*/
    list<unsigned int> qs = {100000, 1000000};
/*  
    list<unsigned int> ns = {1};
    list<unsigned int> Ws = {10000000};
*/
    for (unsigned int q : qs) {
/*
      for (double g : gammas) {
        for (unsigned int n : ns) {
          for (unsigned int W : Ws) {
            benchmark_sqmax_steps(q, g, n, W, &data, sliding_stream);
          }
        }
        benchmark_qmax_steps(q, g, &data, qmax_stream);
*/
	benchmark_heap_steps(q, &data, heap_stream);
	benchmark_skiplist_steps(q, &data, skiplist_stream);
      }
    }
/*
  }
  sliding_stream.close();
  qmax_stream.close();
*/
  heap_stream.close();
  skiplist_stream.close();
  return 0;
}
