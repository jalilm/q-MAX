#include "QmaxKV.hpp"
#include <iostream>

QMaxKV::QMaxKV(int q, float gamma) {
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
	_phi = 0;
	_k_phi = 0;
}

QMaxKV::~QMaxKV() {
	free(_K);
	free(_V);
}

val QMaxKV::getMinimalVal() {
	return _phi;
}

key QMaxKV::getMinimalKey() {
	return _k_phi;
}

void QMaxKV::update(key k, val v) {
	for(int i=0; i < _actualsize; ++i) {
		if(_K[i] == k) {
			_V[i] = v;
			return;
		}
	}
}

void QMaxKV::insert(key k, val v) {
	if (v < _phi) { // DIRECT COMPARSION: compare if new val is smaller than smallest val
		return;
	} else {
		_K[--_curIdx] = k;
		_V[_curIdx] = v;
		if (_curIdx){
			return;
		} else {
			maintenance();
		}
	}
}

void QMaxKV::maintenance() {
	_phi = findKthLargestAndPivot();
	_curIdx = _nminusq;
}

outputkv QMaxKV::largestQ() {
	outputkv out;
	maintenance();
	out.keyArr = _K + _nminusq;
	out.valArr = _V + _nminusq;
	return out;
}

inline void QMaxKV::swap(int a, int b) {
	if (a==b) return;
	key k = _K[a];
	_K[a] = _K[b];
	_K[b] = k;
	val v = _V[a];
	_V[a] = _V[b];
	_V[b] = v;
}
int QMaxKV::PartitionAroundPivot(int left, int right, int pivot_idx, val* nums) {
	val pivot_value = nums[pivot_idx];
	int new_pivot_idx = right;
	swap(pivot_idx, right);
	for (int i = right-1; i >= left; --i) {
		if (nums[i] > pivot_value) { // DIRECT COMPARSION: compare if nums[i] is bigger than pivot_value
			swap(i, --new_pivot_idx);
		}
	}
	swap(right, new_pivot_idx);
	return new_pivot_idx;
/*  
  val pivot_value = nums[pivot_idx];
           int i=left, j=right;
        while (i<=j) {
            while (nums[i]<pivot_value) i++;
            while (nums[j]>pivot_value) j--;
            if (i<=j) swap(i++, j--);
        }
        return i-1;
*/
}

val QMaxKV::findKthLargestAndPivot() {
	int left = 0, right = _actualsizeMinusOne;
	while (left <= right) {
		int pivot_idx = _q;
		int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, _V);
		if (new_pivot_idx == _nminusq) {
			_k_phi = _K[new_pivot_idx];
			return _V[new_pivot_idx];
		} else if (new_pivot_idx > _nminusq) {
			right = new_pivot_idx - 1;
		} else {  // new_pivot_idx < _q - 1.
			left = new_pivot_idx + 1;
		}
	}
}

