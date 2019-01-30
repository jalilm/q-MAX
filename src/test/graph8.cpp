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

void benchmark_sqmax_steps(int q, double gamma, int n, int W, int** data, ofstream& ostream) {
  int* elements = *data;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time = 0;
  SlidingQMax sqmax = SlidingQMax(W, q, gamma, 1.0/n);
  begint = clock();
  ftime(&begintb);
    for (int i = 0; i < N; ++i) {
      sqmax.insert(elements[i]);
    }
  endt = clock();
  ftime(&endtb);
  time += ((double)(endt-begint))/CLK_PER_SEC;
  ostream << "random,Sliding Window q-max," << N << "," << q << "," << gamma << "," << W << "," << double(1.0/n) << "," << time << endl;
}

int main() {
  ofstream sliding_stream;
  setupOutputFile("../results/stiming_random.raw_res", sliding_stream, true);

  for (int run = 0; run < 5; run++) {
    int* data = (int*) malloc(sizeof(int) * N);
    int* vals = (int*) malloc(sizeof(int) * N);
    for (int i = 0; i< N; ++i){
      data[i] = std::rand();
      vals[i] = std::rand();
    }
    list<double> gammas = {0.1, 0.05, 0.01, 0.005, 0.001};
    list<unsigned int> qs = {1000000};
    list<unsigned int> ns = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    list<unsigned int> Ws = {50000000, 100000000};
    for (unsigned int q : qs) {
      for (double g : gammas) {
        for (unsigned int n : ns) {
          for (unsigned int W : Ws) {
            benchmark_sqmax_steps(q, g, n, W, &data, sliding_stream);
          }
        }
      }
    }
  }
  sliding_stream.close();
  return 0;
}
