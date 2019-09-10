#include "qmax.hpp"
#include <iostream>

void QMax::print(){
	for (int i = 0; i < _actualsize; ++i)
		std::cout << _A[i] << " ";
	std::cout << std::endl;
	std::cout << "_phi  = " << _phi << std::endl;
}

QMax::QMax(int q, float gamma){
	_actualsize = q * (1+gamma);
	_actualsizeMinusOne = _actualsize - 1;
	_curIdx = _actualsize;
	_A = (int*) malloc(sizeof(int) * _actualsize);
	if (!_A)
		exit(1);
	_gamma = gamma;
	_q = q;
	_qMinusOne = q - 1;
	_nminusq = _actualsize - q;
	_phi = -1;
//	std::cout << _actualsize << " " << _q << " " << _gamma << std::endl;
}
void QMax::insert(int v){
	if (v < _phi){
		return;
	}
	else{
		_A[--_curIdx] = v;
		if (_curIdx){
			//std::cout << _curIdx << " " << _A[_curIdx] << " ";
			return;
		}
		else {
			maintenance();
		}
	}
}

void QMax::maintenance(){
	_phi = findKthLargestAndPivot();
	_curIdx = _nminusq;
}

int* QMax::largestQ(){
	maintenance();
	//cout 
	return _A + _nminusq;
}

inline void swap(int &x, int &y){
	int z = x;
	x = y;
	y = z;
}
int QMax::PartitionAroundPivot(int left, int right, int pivot_idx, int* nums) {
	//auto& nums_ref = *nums;
	int pivot_value = nums[pivot_idx];
	int new_pivot_idx = right;
	//std::cout << "pivot_value = " << pivot_value << std::endl;
	swap(nums[pivot_idx], nums[right]);
	//print();
	for (int i = right-1; i >= left; --i) {
		if (nums[i] > pivot_value) {
			swap(nums[i], nums[--new_pivot_idx]);
		}
	}
	swap(nums[right], nums[new_pivot_idx]);
	return new_pivot_idx;
}

int QMax::findKthLargestAndPivot() {
	int left = 0, right = _actualsizeMinusOne;
	while (left <= right) {
		int pivot_idx = left;
		int new_pivot_idx = PartitionAroundPivot(left, right, pivot_idx, _A);
		//std::cout << left << " " << right << std::endl;
		if (new_pivot_idx == _nminusq) {
			return _A[new_pivot_idx];
		} else if (new_pivot_idx > _nminusq) {
			right = new_pivot_idx - 1;
		} else {  // new_pivot_idx < _q - 1.
			left = new_pivot_idx + 1;
		}
	}
}

void QMax::reset(){
	_phi = -1;
	_curIdx = _actualsize;
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
