#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "../QmaxKV.hpp"
#include "Utils.hpp"

#define N 150000000
#define CLK_PER_SEC CLOCKS_PER_SEC

using namespace std;

void benchmark_qmaxkv(int q, double gamma, key** data, val** vals, ofstream &ostream) {
  key* elements = *data;
  val* ve = *vals;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMaxKV qmax(q, gamma);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < N; i++) {
    qmax.insert(elements[i], ve[i]);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,AmortizedQMax," << N << "," << q << "," << gamma << "," << time << endl;
}

int main() {
  ofstream ostream;
  setupOutputFile("../results/timing_randomKV.raw_res", ostream, false);
  for (int run = 0; run < 4; run++) {
    key* data = (key*) malloc(sizeof(key) * N);
    val* vals = (val*) malloc(sizeof(val) * N);
    for (int i = 0; i< N; ++i){
      data[i] = std::rand();
      vals[i] = std::rand();
    }
    list<int> qs = {1000, 100000, 1000000, 10000000};
    for (int q : qs) {
      list<double> gammas = {2,1,0.5,0.25,0.1,0.05,0.01,0.005};
      for (double g : gammas) {
        benchmark_qmaxkv(q, g, &data, &vals, ostream);
      }
    }
  }
  ostream.close();
  return 0;
}
