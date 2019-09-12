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

#include <time.h>
#include <stdlib.h>

#include <valgrind/callgrind.h>

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

void benchmark_pbaskiplist(int q, key** keys, val** ws, int numKeys) {
  srand(12345);
  key *elements = *keys;
  val *vals = *ws;
  SkiplistIn<key, pba_val> sl(q, 0, pba_val());
  init_pba();
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!sl.isIn(k)) {
        double r = ((double)rand()) / ((double)RAND_MAX);
        pba_val new_val(x, x, 1, 1-r);
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
}

void benchmark_pbaheap(int q, key** keys, val** ws, int numKeys) {
  srand(12345);
  key *elements = *keys;
  val *vals = *ws;
  HeapIn<key, pba_val> heap(q, 0, pba_val());
  init_pba();
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!heap.isIn(k)) {
	double r = ((double)rand()) / ((double)RAND_MAX);
        pba_val new_val(x, x, 1, 1-r);
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
}

void benchmark_pbaqmax(int q, double gamma, key** keys, val** ws,int numKeys) {
  srand(12345);
  key *elements = *keys;
  val *vals = *ws;
  QMaxIn<key, pba_val> qmax = QMaxIn<key, pba_val>(q, gamma, 0, pba_val());
  init_pba();//z_star = 0;
  for (int i = 0; i < numKeys; i++) {
    key k = elements[i];
    val x = vals[i];
    if(!qmax.isIn(k)){
	double r = ((double)rand()) / ((double)RAND_MAX);
        pba_val new_val(x, x, 1, 1-r);
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
}

int main() {
  int N = 150000000;
  key* keys = (key*) malloc(sizeof(key) * N);
  val* vals = (val*) malloc(sizeof(val) * N);
  for (int i = 0; i<N; ++i){
    keys[i] = std::rand();
    vals[i] = std::rand();
  }

  // Uninteresting program chunk
  CALLGRIND_START_INSTRUMENTATION;

  list<unsigned int> qs = {10000};
  for (unsigned q: qs) {
    CALLGRIND_TOGGLE_COLLECT;
    benchmark_pbaheap(q, &keys, &vals, N);
    benchmark_pbaskiplist(q, &keys, &vals, N);
    list<double> gammas = {0.05};
    for (double g : gammas) {
      benchmark_pbaqmax(q, g, &keys, &vals, N);
    }
    CALLGRIND_TOGGLE_COLLECT;
  }
  CALLGRIND_DUMP_STATS;
  CALLGRIND_STOP_INSTRUMENTATION;
  return 0;
}

