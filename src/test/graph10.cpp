#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <cmath>

#include "../QmaxKV.hpp"
#include "../HeapKV.hpp"
#include "../SkiplistKV.hpp"
#include "Utils.hpp"

#define N 150000000
#define CLK_PER_SEC CLOCKS_PER_SEC

using namespace std;

void benchmark_skiplistkv(int q, double c, key** data, val** vals, ofstream &ostream) {
  double logc = log(c);
  key* elements = *data;
  val* ve = *vals;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  SkiplistKV sl(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    sl.insert(elements[i], log(ve[i])-i*logc);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,SkipList," << N << "," << q << ",," << c << "," << time << endl;
}

void benchmark_heapkv(int q, double c, key** data, val** vals, ofstream &ostream) {
  double logc = log(c);
  key* elements = *data;
  val* ve = *vals;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  HeapKV heap(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    heap.insert(elements[i], log(ve[i])-i*logc);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,Heap," << N << "," << q << ",," << c << "," << time << endl;
}


void benchmark_qmaxkv(int q, double gamma, double c, key** data, val** vals, ofstream &ostream) {
  double logc = log(c);
  key* elements = *data;
  val* ve = *vals;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMaxKV qmax(q, gamma);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    qmax.insert(elements[i], log(ve[i])-i*logc);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,AmortizedQMax," << N << "," << q << "," << gamma << "," << c << "," << time << endl;
}

int main() {
  ofstream ostream;
  double c = 0.75;
  setupOutputFile("../results/ed-0.75.raw_res", ostream, false, true);
  for (int run = 0; run < 5; run++) {
    key* data = (key*) malloc(sizeof(key) * N);
    val* vals = (val*) malloc(sizeof(val) * N);
    for (int i = 0; i< N; ++i){
      data[i] = std::rand();
      vals[i] = std::rand();
    }
    list<int> qs = {10000, 100000, 1000000, 10000000};
    for (int q : qs) {
//      benchmark_skiplistkv(q, c, &data, &vals, ostream);
//      benchmark_heapkv(q, c, &data, &vals, ostream);
      list<double> gammas = {4,2,1,0.5};//{4,2,1,0.5,0.25,0.1,0.05};
      for (double g : gammas) {
        benchmark_qmaxkv(q, g, c, &data, &vals, ostream);
      }
    }
  }
  ostream.close();
  return 0;
}

