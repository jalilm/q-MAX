#ifndef QMAXIN_H
#define QMAXIN_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <functional>

using namespace std;

template<typename key, typename val>
class QMaxIn {
  key *_K;
  val *_V;
  val _phi;
  key _k_phi;
  unordered_map<key, int> hash_table;

  int _curIdx;
  int _q;
  int _actualsize;
  int _actualsizeMinusOne;
  int _qMinusOne;
  float _gamma;
  int _nminusq;

  void maintenance(){
    findKthLargestAndPivot();
    hash_table.clear();
    for(int i = _nminusq; i < _actualsize; i++) {
      hash_table[_K[i]] = i;
    }
    _curIdx = _nminusq;
  }
  inline void swap(int a, int b){
    if(a==b) return;
    key k = _K[a];
    _K[a] = _K[b];
    _K[b] = k;
    val v = _V[a];
    _V[a] = _V[b];
    _V[b] = v;
  }
  int PartitionAroundPivot(int left, int right, int pivot_idx, val* nums){
    val pivot_value = nums[pivot_idx];
    /*int new_pivot_idx = right;
    swap(pivot_idx, right);
    for (int i = right-1; i >= left; --i) {
      if (nums[i] > pivot_value) { // DIRECT COMPARSION: compare if nums[i] is bigger than pivot_value
        swap(i, --new_pivot_idx);
      }
    }
    swap(right, new_pivot_idx);
    return new_pivot_idx;*/
           int i=left, j=right;
        while (i<=j) {
            while (nums[i]<pivot_value) i++;
            while (nums[j]>pivot_value) j--;
            if (i<=j) swap(i++, j--);
        }

        // return new position of pivot
        return i-1;
  }
  void findKthLargestAndPivot(){
    int left = 0, right = _actualsizeMinusOne;
    while (left <= right) {
      int pivot_idx = (left+right)/2;
      int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, _V);
      if (new_pivot_idx == _nminusq) {
        _phi = _V[new_pivot_idx];
        _k_phi = _K[new_pivot_idx];
        return;
      } else if (new_pivot_idx > _nminusq) {
        right = new_pivot_idx - 1;
      } else {  // new_pivot_idx < _q - 1.
        left = new_pivot_idx + 1;
      }
    }
  }
 public:
  template<typename _key, typename _val>
  class outputkv {
  public:
    _key *keyArr;
    _val *valArr;
  };
  QMaxIn(int q, float gamma, key default_key=0, val default_val=0){
    _actualsize = q * (1+gamma);
    _actualsizeMinusOne = _actualsize - 1;
    _curIdx = _actualsize;
    _K = (key*) malloc(sizeof(key) * _actualsize);
    _V = (val*) malloc(sizeof(val) * _actualsize);
    if (!_V || !_K) {
      exit(1);
    }
    _gamma = gamma;
    _q = q;
    _qMinusOne = q - 1;
    _nminusq = _actualsize - q;
    _phi = default_val;
    _k_phi = default_key;
  }
  ~QMaxIn(){
    free(_K);
    free(_V);
  }
  bool isIn(key k) {
    return hash_table.count(k);
  }
  val getMinimalVal() {
    return _phi;
  }
  key getMinimalKey() {
    return _k_phi;
  }
  void insert(key k, val v){
    if (v < _phi) { // DIRECT COMPARSION: compare if new val is smaller than smallest val
      return;
    } else {
      _K[--_curIdx] = k;
      _V[_curIdx] = v;   
      hash_table[k] = _curIdx;
      if (_curIdx){
        return;
      } else {
        maintenance();
      }
    }

  }
  void update(key k, val v, std::function<void(val&,val&)> f = [](val& v1, val& v2) {v1+=v2;}) {
    int k_idx = hash_table[k];
    f(_V[k_idx], v);
  }
  outputkv<key, val> largestQ(){
    outputkv<key,val> out;
    maintenance();
    out.keyArr = _K + _nminusq;
    out.valArr = _V + _nminusq;
    return out;
  }
};

#endif

