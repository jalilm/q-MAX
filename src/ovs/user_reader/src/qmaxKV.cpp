#include "qmaxKV.hpp"
#include <iostream>

void QMaxKV::print(){
	for (int i = 0; i < _actualsize; ++i)
		std::cout << _K[i] << "," << _V[i] << " ";
	std::cout << std::endl;
	std::cout << "_phi  = " << _phi << std::endl;
}

QMaxKV::QMaxKV(int q, float gamma){
	_actualsize = q * (1+gamma);
	_actualsizeMinusOne = _actualsize - 1;
	_curIdx = _actualsize;
	_K = (key*) malloc(sizeof(key) * _actualsize);
	_V = (key*) malloc(sizeof(val) * _actualsize);
	if (!_V)
		exit(1);
	_gamma = gamma;
	_q = q;
	_qMinusOne = q - 1;
	_nminusq = _actualsize - q;
	_phi = 0;
//	std::cout << _actualsize << " " << _q << " " << _gamma << std::endl;
}
void QMaxKV::insert(key k, val v){
	if (k < _phi){
		return;
	}
	else{
		_K[--_curIdx] = k;
		_V[_curIdx] = v;
		if (_curIdx){
			//std::cout << _curIdx << " " << _A[_curIdx] << " ";
			return;
		}
		else {
			maintenance();
		}
	}
}

void QMaxKV::maintenance(){
	_phi = findKthLargestAndPivot();
	_curIdx = _nminusq;
}



outputkv QMaxKV::largestQ(){
	outputkv out;
	maintenance();
	out.keyArr = _K + _nminusq;
	out.valArr = _V + _nminusq;
	//cout 
	return out;
}

inline void QMaxKV::swap(int a, int b){
	key k = _K[a];
	_K[a] = _K[b];
	_K[b] = k;
	val v = _V[a];
	_V[a] = _V[b];
	_V[b] = v;
}
int QMaxKV::PartitionAroundPivot(int left, int right, int pivot_idx, key* nums) {
	//auto& nums_ref = *nums;
	key pivot_value = nums[pivot_idx];
	int new_pivot_idx = right;
	//std::cout << "pivot_value = " << pivot_value << std::endl;
	swap(pivot_idx, right);
	//print();
	for (int i = right-1; i >= left; --i) {
		if (nums[i] > pivot_value) {
			swap(i, --new_pivot_idx);
		}
	}
	swap(right, new_pivot_idx);
	return new_pivot_idx;
}

int QMaxKV::findKthLargestAndPivot() {
	int left = 0, right = _actualsizeMinusOne;
	while (left <= right) {
		int pivot_idx = left;
		int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, _K);
		//std::cout << left << " " << right << std::endl;
		if (new_pivot_idx == _nminusq) {
			return _K[new_pivot_idx];
		} else if (new_pivot_idx > _nminusq) {
			right = new_pivot_idx - 1;
		} else {  // new_pivot_idx < _q - 1.
			left = new_pivot_idx + 1;
		}
	}
}
/*int findKthLargestAndPivot(int* nums, int n, int k) {
	int left = 0, right = n - 1;
	//default_random_engine gen((random_device())());
	while (left <= right) {
		// Generates a random int in [left, right].
		//uniform_int_distribution<int> dis(left, right);
		//int pivot_idx = dis(gen);
		int pivot_idx = left;
		int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, nums);
		std::cout << left << " " << right << std::endl;
		if (new_pivot_idx == k - 1) {
			return nums[new_pivot_idx];
		} else if (new_pivot_idx > k - 1) {
			right = new_pivot_idx - 1;
		} else {  // new_pivot_idx < k - 1.
			left = new_pivot_idx + 1;
		}
	}
}*/
