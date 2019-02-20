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

#include "../QmaxIn.hpp"
#include "../HeapIn.hpp"
#include "../SkiplistIn.hpp"
#include "Utils.hpp"

#define CLK_PER_SEC CLOCKS_PER_SEC
#define CAIDA16_SIZE 10832820 //141847914
#define CAIDA18_SIZE 10832820 //132356631
#define UNIV1_SIZE 10832820

using namespace std;

typedef double val;
typedef unsigned long long key;

class pba_val {
  public:
	pba_val() : w(0), a(0), q(0), u(0), w_by_u(0), current_z_star(0) {}
	pba_val(double _w, double _a, double _q, double _u, double _z = 0 ) : w(_w), a(_a), q(_q), u(_u), w_by_u(w/u), current_z_star(_z) {}
        double w;
        double a;
        double q;
        double u;
        val w_by_u;
	double current_z_star;
	inline bool operator< (const pba_val& rhs) const { return this->w_by_u < rhs.w_by_u; }
	inline bool operator> (const pba_val& rhs) const { return rhs < *this; }
	inline bool operator<=(const pba_val& rhs) const { return !(*this > rhs); }
	inline bool operator>=(const pba_val& rhs) const { return !(*this < rhs); }
	inline bool operator==(const pba_val& rhs) const { return this->w_by_u == rhs.w_by_u; }
	inline bool operator!=(const pba_val& rhs) const { return !(*this == rhs); }
	inline pba_val& operator+=(const pba_val& other) {
		this->a *= this->q;
	        if(other.current_z_star != 0) {
                	double w_by_z_star = this->w / other.current_z_star;
                	this->q = this->q < (w_by_z_star) ? this->q : w_by_z_star;
        	}
        	if(this->q != 0) {
                	this->a /= this->q;
        	}
		this->a += other.w;
        	this->w += other.w;
        	this->w_by_u = this->w / this->u;
		return *this;
	}
};

double z_star;

void init_pba() {
	z_star = 0;
}

void update_zstar_with_minimal(pba_val v) {
        z_star = (z_star > v.w_by_u) ? z_star : v.w_by_u;
}

void update_with_zstar(pba_val &h) {
	h.a *= h.q;
	if(z_star != 0) {
                double w_by_z_star = h.w / z_star;
                h.q = h.q < (w_by_z_star) ? h.q : w_by_z_star;
        }
        if(h.q != 0) {
                h.a /= h.q;
        }
}

void final_step_on_all_keys(QMaxIn<key, pba_val>::outputkv<key, pba_val> okv, int q) {
	for(int i = 0; i < q; ++i) {
		update_with_zstar(okv.valArr[i]);
	}
}

void final_step_on_all_pairs(vector<pair<key, pba_val>> v) {
	for (auto &it : v) {
		update_with_zstar(it.second);
	}
}

void benchmark_pbaskiplist(int q, key** keys, val** ws, ofstream &ostream, string dataset, int numKeys) {
  random_device _rd;
  mt19937 _e2(_rd());
  uniform_real_distribution<double> _dist(0,1);
  key *elements = *keys;
  val *vals = *ws;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  SkiplistIn<key, pba_val> sl(q, 0, pba_val());
  init_pba();
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!sl.isIn(k)) {
        pba_val new_val(x, x, 1, 1-_dist(_e2));
        pba_val min_val= sl.getMinimalVal();
        if (new_val < min_val) {
                update_zstar_with_minimal(new_val);
        } else {
                update_zstar_with_minimal(min_val);
                sl.insert(k, new_val);
        }
    } else {
        pba_val new_val(x, x, 1, 1, z_star);
        sl.update(k, new_val);
    }
  }
  final_step_on_all_pairs(sl.getItems());
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",SkipList," << numKeys << "," << q << ",," << time << endl;
}

void benchmark_pbaheap(int q, key** keys, val** ws, ofstream &ostream, string dataset, int numKeys) {
  random_device _rd;
  mt19937 _e2(_rd());
  uniform_real_distribution<double> _dist(0,1);
  key *elements = *keys;
  val *vals = *ws;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  HeapIn<key, pba_val> heap(q, 0, pba_val());
  init_pba();
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!heap.isIn(k)) {
        pba_val new_val(x, x, 1, 1-_dist(_e2));
        pba_val min_val= heap.getMinimalVal();
        if (new_val < min_val) {
                update_zstar_with_minimal(new_val);
        } else {
		update_zstar_with_minimal(min_val);
                heap.insert(k, new_val);
        }
    } else {
	pba_val new_val(x, x, 1, 1, z_star);
        heap.update(k, new_val);
    }
  }
  final_step_on_all_pairs(heap.getItems());
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",Heap," << numKeys << "," << q << ",," << time << endl;
}

void benchmark_pbaqmax(int q, double gamma, key** keys, val** ws, ofstream &ostream, string dataset, int numKeys) {
  random_device _rd;
  mt19937 _e2(_rd());
  uniform_real_distribution<double> _dist(0,1);
  key *elements = *keys;
  val *vals = *ws;
  struct timeb begintb, endtb;
  clock_t begint, endt;
  double time;
  QMaxIn<key, pba_val> qmax = QMaxIn<key, pba_val>(q, gamma, 0, pba_val());
  init_pba();//z_star = 0;
  begint = clock();
  ftime(&begintb);
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!qmax.isIn(k)){
	pba_val new_val(x, x, 1, 1-_dist(_e2));
	pba_val min_val= qmax.getMinimalVal();
        if (new_val < min_val) {
		update_zstar_with_minimal(new_val);
        } else {
		update_zstar_with_minimal(min_val);
		qmax.insert(k, new_val);
	}
    } else {
	pba_val new_val(x, x, 1, 1, z_star);
	qmax.update(k, new_val);
    }
  }
  final_step_on_all_keys(qmax.largestQ(), q);
  endt = clock();
  ftime(&endtb);
  time = ((double)(endt-begint))/CLK_PER_SEC;
  ostream << dataset << ",AmortizedQMax," << numKeys << "," << q << "," << gamma << "," << time << endl;
}

void getKeysAndWeightsFromFile(string filename, vector<key*> &keys, vector<val*> &ws, int size) {
  ifstream stream;
  stream.open(filename, fstream::in | fstream::out | fstream::app);
  if (!stream) {
    throw invalid_argument("Could not open " + filename + " for reading.");
  }

  key* file_keys = (key*) malloc(sizeof(key) * size);
  val* file_ws = (val*) malloc(sizeof(val) * size);

  string line;
  string len;
  string id;
  for (int i = 0; i < size; ++i){
    getline(stream, line);
    std::istringstream iss(line);
    iss >> len;
    iss >> id;
    try {
      file_keys[i] = stoull(id);
      file_ws[i] = stod(len);
    } catch (const std::invalid_argument& ia) {
      cerr << "Invalid argument: " << ia.what() << " at line " << i << endl;
      cerr << len << " " << id << endl;;
      --i;
      exit(1);
    }
  }
  keys.push_back(file_keys);
  ws.push_back(file_ws);

  stream.close();
}

int main() {
  vector<ofstream*> streams;
  vector<key*> keys;
  vector<val*> ws;
  vector<int> sizes;
  vector<string> datasets;
/*
  ofstream univ1stream;
  setupOutputFile("../results/pba_univ1.raw_res", univ1stream, false);
  streams.push_back(&univ1stream);
  getKeysAndWeightsFromFile("../datasets/UNIV1/mergedAggregatedPktlen_Srcip", keys, ws, UNIV1_SIZE);
  sizes.push_back(UNIV1_SIZE);
  datasets.push_back("univ1");
  cout << "read UNIV1" << endl;
  */
  ofstream caida16stream;
  setupOutputFile("../results/pba_caida.raw_res", caida16stream, false);
  streams.push_back(&caida16stream);
  getKeysAndWeightsFromFile("../datasets/CAIDA16/mergedAggregatedPktlen_Srcip", keys, ws, CAIDA16_SIZE);
  sizes.push_back(CAIDA16_SIZE);
  datasets.push_back("caida");
  cout << "read CAIDA16" << endl;
  
  ofstream caida18stream;
  setupOutputFile("../results/pba_caida18.raw_res", caida18stream, false);
  streams.push_back(&caida18stream);
  getKeysAndWeightsFromFile("../datasets/CAIDA18/mergedAggregatedPktlen_Srcip", keys, ws, CAIDA18_SIZE);
  sizes.push_back(CAIDA18_SIZE);
  datasets.push_back("caida18");
  cout << "read CAIDA18" << endl;
  
  list<unsigned int> qs = {1000000};
  for (int run = 0; run < 1; run++) {
  for (unsigned q: qs) {
    vector<key*>::iterator k_it = keys.begin();
    vector<val*>::iterator w_it = ws.begin();
    vector<int>::iterator s_it = sizes.begin();
    vector<string>::iterator d_it = datasets.begin();
    for (auto& stream : streams) {
      key* k = *k_it;
      val* w = *w_it;
      int size = *s_it;
      string dataset = *d_it;
      benchmark_pbaheap(q, &k, &w, *stream, dataset, size);
//      benchmark_pbaskiplist(q, &k, &w, *stream, dataset, size);
      list<double> gammas = {0.05, 0.1, 0.25, 0.5};
      for (double g : gammas) {
//        benchmark_pbaqmax(q, g, &k, &w, *stream, dataset, size);
      }
      ++k_it;
      ++w_it;
      ++s_it;
      ++d_it;
    }
  }
  }
//  univ1stream.close();
  caida16stream.close();
  caida18stream.close();
  return 0;
}

