#include <cstdio>
#include <cstdlib>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <random>
#include <unordered_map>

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <cmath>

#include "../QmaxIn.hpp"
#include "../HeapIn.hpp"
#include "../SkiplistIn.hpp"
#include "Utils.hpp"

#define CLK_PER_SEC CLOCKS_PER_SEC
#define CACHE_SIZE 1814969

using namespace std;

typedef unsigned long long key;

class lrfu_val {
  public:
	lrfu_val() : _log_w(0) {}
	lrfu_val(double log_w) : _log_w(log_w) {}
        double _log_w;
	inline bool operator< (const lrfu_val& rhs) const { return this->_log_w < rhs._log_w; }
	inline bool operator> (const lrfu_val& rhs) const { return rhs < *this; }
	inline bool operator<=(const lrfu_val& rhs) const { return !(*this > rhs); }
	inline bool operator>=(const lrfu_val& rhs) const { return !(*this < rhs); }
	inline bool operator==(const lrfu_val& rhs) const { return this->_log_w == rhs._log_w; }
	inline bool operator!=(const lrfu_val& rhs) const { return !(*this == rhs); }
	inline lrfu_val& operator+=(const lrfu_val& other) {
		double w2 = other._log_w;
		double w1 = this->_log_w;
		this->_log_w = w2 + log(exp(w1-w2) + 1);
		return *this;
	}
};

double logc;

void init_lrfu(double c) {
	logc = log(c);
}

void benchmark_lrfuskiplist(int q, double c, key** keys, ofstream &ostream, string dataset, int numKeys) {
  key *elements = *keys;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  SkiplistIn<key, lrfu_val> sl(q, 0, lrfu_val());
  init_lrfu(c);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!sl.isIn(k)) {
	sl.insert(k, new_val);
    } else {
        sl.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",SkipList," << numKeys << "," << q << ",," << c << "," << time << endl;
}

void benchmark_lrfuheap(int q, double c, key** keys, ofstream &ostream, string dataset, int numKeys) {
  key *elements = *keys;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  HeapIn<key, lrfu_val> heap(q, 0, lrfu_val());
  init_lrfu(c);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!heap.isIn(k)) {
	heap.insert(k, new_val);
    } else {
        heap.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",Heap," << numKeys << "," << q << ",," << c << "," << time << endl;
}

void benchmark_lrfuqmax(int q, double gamma, double c, key** keys, ofstream &ostream, string dataset, int numKeys) {
  key *elements = *keys;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMaxIn<key, lrfu_val> qmax = QMaxIn<key, lrfu_val>(q, gamma, 0, lrfu_val());
  init_lrfu(c);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!qmax.isIn(k)){
	qmax.insert(k, new_val);
    } else {
	qmax.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",AmortizedQMax," << numKeys << "," << q << "," << gamma << "," << c << "," << time << endl;
}

void getKeysFromFile(string filename, vector<key*> &keys, int size) {
  ifstream stream;
  stream.open(filename, fstream::in | fstream::out | fstream::app);
  if (!stream) {
    throw invalid_argument("Could not open " + filename + " for reading.");
  }

  key* file_keys = (key*) malloc(sizeof(key) * size);

  string line;
  string id;
  for (int i = 0; i < size; ++i){
    getline(stream, line);
    std::istringstream iss(line);
    iss >> id;
    try {
      file_keys[i] = stoull(id);
    } catch (const std::invalid_argument& ia) {
      cerr << "Invalid argument: " << ia.what() << " at line " << i << endl;
      cerr << id << endl;;
      --i;
      exit(1);
    }
  }
  keys.push_back(file_keys);
  stream.close();
}

int main() {
  vector<ofstream*> streams;
  vector<key*> keys;
  vector<int> sizes;
  vector<string> datasets;

  ofstream cachestream;
  setupOutputFile("../results/lrfu_P1.raw_res", cachestream, false, true);
  streams.push_back(&cachestream);
  getKeysFromFile("../datasets/CACHE/P1.list", keys, CACHE_SIZE);
  sizes.push_back(CACHE_SIZE);
  datasets.push_back("cache");
  cout << "read CACHE" << endl;
  
  list<unsigned int> qs = {10000, 100000, 1000000};
  list<double> cs = {0.75};
  for (int run = 0; run < 9; run++) {
  for (unsigned q: qs) {
  for (double c: cs) {
    vector<key*>::iterator k_it = keys.begin();
    vector<int>::iterator s_it = sizes.begin();
    vector<string>::iterator d_it = datasets.begin();
    for (auto& stream : streams) {
      key* k = *k_it;
      int size = *s_it;
      string dataset = *d_it;
      benchmark_lrfuheap(q, c, &k, *stream, dataset, size);
      benchmark_lrfuskiplist(q, c, &k, *stream, dataset, size);
      list<double> gammas = {0.05, 0.1, 0.25, 0.5, 1, 2};
      for (double g : gammas) {
        benchmark_lrfuqmax(q, g, c, &k, *stream, dataset, size);
      }
      ++k_it;
      ++s_it;
      ++d_it;
    }
  }
  }
  }
  cachestream.close();
  return 0;
}

