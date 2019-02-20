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
  int hits = 0;
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!sl.isIn(k)) {
	sl.insert(k, new_val);
    } else {
	++hits;
        sl.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",SkipList," << numKeys << "," << q << ",," << c << "," << double(hits)/numKeys << endl;
}

void benchmark_lrfuskiplist_g(int q, double g, double c, key** keys, ofstream &ostream, string dataset, int numKeys) {
  key *elements = *keys;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  SkiplistIn<key, lrfu_val> sl(q*(1+g), 0, lrfu_val());
  init_lrfu(c);
  begint = clock();
  ftime(&begintb);
  int hits = 0;
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!sl.isIn(k)) {
        sl.insert(k, new_val);
    } else {
        ++hits;
        sl.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",SkipList-g," << numKeys << "," << q << "," << g << "," << c << "," << double(hits)/numKeys << endl;
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
  int hits = 0;
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    lrfu_val new_val(-i*logc);
    if(!qmax.isIn(k)){
	qmax.insert(k, new_val);
    } else {
	++hits;
	qmax.update(k, new_val);
    }
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",AmortizedQMax," << numKeys << "," << q << "," << gamma << "," << c << "," << double(hits)/numKeys << endl;
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
  vector<string> datasets;

  vector<int> sizes = {1814969};//, 666176, 238578, 967169, 1118323, 519711, 751473, 547147};
  list<string> traces = {"P1"};//, "P2", "P3", "P4", "P5", "P6", "P7", "P12"};
  vector<int>::iterator s_it = sizes.begin();

  for (string t : traces) {
  int CACHE_SIZE = *s_it;
  ++s_it;
  ofstream cachestream;
  setupOutputFile("../results/" + t + "-hitrate.raw_res", cachestream, false, true);
  streams.push_back(&cachestream);
  getKeysFromFile("../datasets/CACHE/" + t + ".lis", keys, CACHE_SIZE);
  datasets.push_back("cache");
  cout << "read " + t + " CACHE" << endl;
  
  list<unsigned int> qs = {10000, 100000};
  list<double> cs = {0.75};
  for (int run = 0; run < 1; run++) {
  for (unsigned q: qs) {
  for (double c: cs) {
    vector<key*>::iterator k_it = keys.begin();
    vector<string>::iterator d_it = datasets.begin();
    for (auto& stream : streams) {
      key* k = *k_it;
      string dataset = *d_it;
      benchmark_lrfuskiplist(q, c, &k, *stream, dataset, CACHE_SIZE);
      list<double> gammas = {0.1, 0.5, 1};
      for (double g : gammas) {
	benchmark_lrfuskiplist_g(q, g, c, &k, *stream, dataset, CACHE_SIZE);
        benchmark_lrfuqmax(q, g, c, &k, *stream, dataset, CACHE_SIZE);
      }
      ++k_it;
      ++d_it;
    }
  }
  }
  }
  cachestream.close();
  }
  return 0;
}

