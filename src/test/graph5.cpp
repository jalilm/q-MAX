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

#include "../QmaxKV.hpp"
#include "../HeapKV.hpp"
#include "../SkiplistKV.hpp"
#include "Utils.hpp"

#define CLK_PER_SEC CLOCKS_PER_SEC
#define CAIDA16_SIZE 152197437
#define CAIDA18_SIZE 175880808
#define UNIV1_SIZE 17323447

using namespace std;

void benchmark_psskiplist(int q, key** data, val** data2, ofstream &ostream, string dataset, int numKeys) {
  std::random_device _rd;
  std::mt19937 _e2(_rd());
  std::uniform_real_distribution<double> _dist(0,1);
  key *elements = *data;
  val *weights = *data2;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  SkiplistKV sl(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    unsigned long long priority = weights[i] / _dist(_e2);
    sl.add(pair<key, val>(priority, k));
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",SkipList," << numKeys << "," << q << ",," << time << endl;
}

void benchmark_psheap(int q, key** data, val** data2, ofstream &ostream, string dataset, int numKeys) {
  std::random_device _rd;
  std::mt19937 _e2(_rd());
  std::uniform_real_distribution<double> _dist(0,1);
  key *elements = *data;
  val *weights = *data2;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  HeapKV heap(q);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    unsigned long long priority = weights[i] / _dist(_e2);
    heap.add(pair<key,val>(priority, k));
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",Heap," << numKeys << "," << q << ",," << time << endl;
}

void benchmark_psqmax(int q, double gamma, key** data, val** data2, ofstream &ostream, string dataset, int numKeys) {
  std::random_device _rd;
  std::mt19937 _e2(_rd());
  std::uniform_real_distribution<double> _dist(0,1);
  key *elements = *data;
  val *weights = *data2;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMaxKV qmax = QMaxKV(q, gamma);
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    unsigned long long priority = weights[i] / _dist(_e2);
    qmax.insert(priority, k);
  }
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",AmortizedQMax," << numKeys << "," << q << "," << gamma << "," << time << endl;
}

void getKeysFromFile(string filename, vector<key*> &keys, vector<val*> &vals, int size) {
  ifstream stream;
  stream.open(filename, fstream::in | fstream::out | fstream::app);
  if (!stream) {
    throw invalid_argument("Could not open " + filename + " for reading.");
  }

  key* data2 = (key*) malloc(sizeof(key) * size);
  val* data1 = (val*) malloc(sizeof(val) * size);
  string line;
  string cell1;
  string cell2;
  for (int i = 0; i < size; ++i){
    getline(stream, line);
    std::istringstream iss(line);
    iss >> cell1;
    iss >> cell2;
    try {
      data1[i] = stoull(cell1);
      data2[i] = stoull(cell2);
    } catch (const std::invalid_argument& ia) {
      cerr << "Invalid argument: " << ia.what() << " at line " << i << endl;
      cerr << cell1 << " " << cell2 << endl;;
      --i;
      exit(1);
    }
  }
  keys.push_back(data2);
  vals.push_back(data1);

  stream.close();
}

int main() {
  vector<ofstream*> streams;
  vector<key*> keys;
  vector<val*> vals;
  vector<int> sizes;
  vector<string> datasets;

  ofstream univ1stream;
  setupOutputFile("../results/ps_univ1kv.raw_res", univ1stream, false);
  streams.push_back(&univ1stream);
  getKeysFromFile("../datasets/UNIV1/mergedPktlen_Srcip", keys, vals, UNIV1_SIZE);
  sizes.push_back(UNIV1_SIZE);
  datasets.push_back("univ1");

  ofstream caida16stream;
  setupOutputFile("../results/ps_caidakv.raw_res", caida16stream, false);
  streams.push_back(&caida16stream);
  getKeysFromFile("../datasets/CAIDA16/mergedPktlen_Srcip", keys, vals, CAIDA16_SIZE);
  sizes.push_back(CAIDA16_SIZE);
  datasets.push_back("caida");

  ofstream caida18stream;
  setupOutputFile("../results/ps_caida18kv.raw_res", caida18stream, false);
  streams.push_back(&caida18stream);
  getKeysFromFile("../datasets/CAIDA18/mergedPktlen_Srcip", keys, vals, CAIDA18_SIZE);
  sizes.push_back(CAIDA18_SIZE);
  datasets.push_back("caida18");

  list<unsigned int> qs = {10000000, 1000000, 100000, 10000};
  for (int run = 0; run < 1; run++) {
  for (unsigned q: qs) {
    vector<key*>::iterator k_it = keys.begin();
    vector<val*>::iterator v_it = vals.begin();
    vector<int>::iterator s_it = sizes.begin();
    vector<string>::iterator d_it = datasets.begin();
    for (auto& stream : streams) {
      key* k = *k_it;
      val* v = *v_it;
      int size = *s_it;
      string dataset = *d_it;
//      benchmark_psheap(q, &k, &v, *stream, dataset, size);
  //    benchmark_psskiplist(q, &k, &v, *stream, dataset, size);
      list<double> gammas = {0.5, 0.25, 0.1, 0.05, 0.01, 0.005};
      for (double g : gammas) {
        benchmark_psqmax(q, g, &k, &v, *stream, dataset, size);
      }
      ++k_it;
      ++v_it;
      ++s_it;
      ++d_it;
    }
  }
  }
  univ1stream.close();
  caida16stream.close();
  caida18stream.close();
  return 0;
}

