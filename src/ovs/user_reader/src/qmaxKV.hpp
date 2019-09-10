#ifndef QMAXKV_H
#define QMAXKV_H
#include <stdlib.h>
#include <string.h>
#include <iostream>
//#define swap(x, y) x = x^y; y = x^y; x = x^y;
typedef unsigned long long key;
typedef unsigned long long val;
typedef struct output{
	key *keyArr;
	val *valArr;
} outputkv;
class QMaxKV
{
        key *_K;
	val *_V;
	int _curIdx;
	int _q;
	int _actualsize;
	int _actualsizeMinusOne;
	int _qMinusOne;
	float _gamma;
	int _nminusq;
	int _phi;
	void maintenance();
	inline void swap(int a, int b);
	int PartitionAroundPivot(int left, int right, int pivot_idx, key* nums);
public:
	int findKthLargestAndPivot();
	QMaxKV(int q, float gamma);
	void insert(key k, val v);
	outputkv largestQ();
	void print();
};

#endif
