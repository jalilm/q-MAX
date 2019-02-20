#include "QmaxPBAKV.hpp"
#include <iostream>
#include <unordered_map>

using namespace std;

QMaxPBAKV::QMaxPBAKV(int q, float gamma) {
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
	z_star = 0;
}

QMaxPBAKV::~QMaxPBAKV() {
	free(_K);
	free(_V);
}

void QMaxPBAKV::insert(key k, weight x, double u) {
	auto it = hash_table.find(k);
	if (it != hash_table.end()) {
	//outputkv out = largestQ();
	//for(int i=0; i <_q; ++i) {
	//	if(out.keyArr[i] == k) {
			unsigned int i = it->second;
			_V[i].a *= _V[i].q;
			if(z_star != 0) {
				double w_by_z_star = _V[i].w / z_star;
				_V[i].q = _V[i].q < (w_by_z_star) ? _V[i].q : w_by_z_star;
			}
			if(_V[i].q != 0) {
				_V[i].a /= _V[i].q;
			}
			_V[i].a += x;
			_V[i].w += x;
			_V[i].w_by_u = _V[i].w / _V[i].u;
			return;
	//	}
	}
	pba_val v = {x, x, 1, u, x/u};
	if (v.w_by_u < _phi) {
		z_star = (z_star > v.w_by_u) ? z_star : v.w_by_u;
		return;
	} else {
		key key_star = _K[_curIdx];
		weight w_by_u_star = _V[_curIdx].w_by_u;
		z_star = (z_star > w_by_u_star) ? z_star : w_by_u_star;
		_K[--_curIdx] = k;
		_V[_curIdx] = v;
		hash_table[k] = _curIdx;
		if (_curIdx){
			return;
		} else {
			outputkv out = largestQ(); // performs maintenance implicitly
			hash_table.clear();
			for(int i=0; i<_q; ++i) {
				hash_table[out.keyArr[i]] = _nminusq + i;
			}
			//maintenance();
		}
	}
}

void QMaxPBAKV::maintenance() {
	_phi = findKthLargestAndPivot();
	_curIdx = _nminusq;
}

outputkv QMaxPBAKV::largestQ() {
	outputkv out;
	maintenance();
	out.keyArr = _K + _nminusq;
	out.valArr = _V + _nminusq;
	return out;
}

inline void QMaxPBAKV::swap(int a, int b) {
	key k = _K[a];
	_K[a] = _K[b];
	_K[b] = k;
	val v = _V[a];
	_V[a] = _V[b];
	_V[b] = v;
}
int QMaxPBAKV::PartitionAroundPivot(int left, int right, int pivot_idx, val* nums) {
	weight pivot_value = nums[pivot_idx].w_by_u;
	int new_pivot_idx = right;
	swap(pivot_idx, right);
	for (int i = right-1; i >= left; --i) {
		if (nums[i].w_by_u > pivot_value) {
			swap(i, --new_pivot_idx);
		}
	}
	swap(right, new_pivot_idx);
	return new_pivot_idx;
}

weight QMaxPBAKV::findKthLargestAndPivot() {
	int left = 0, right = _actualsizeMinusOne;
	while (left <= right) {
		int pivot_idx = left;
		int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, _V);
		if (new_pivot_idx == _nminusq) {
			return _V[new_pivot_idx].w_by_u;
		} else if (new_pivot_idx > _nminusq) {
			right = new_pivot_idx - 1;
		} else {  // new_pivot_idx < _q - 1.
			left = new_pivot_idx + 1;
		}
	}
}

