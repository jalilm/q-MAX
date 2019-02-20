#ifndef QMAXKV_H
#define QMAXKV_H
#include <stdlib.h>
#include <string.h>
#include <iostream>

typedef unsigned long long key;
typedef double val;

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
	val _phi;
	key _k_phi;
	void maintenance();
	inline void swap(int a, int b);
	int PartitionAroundPivot(int left, int right, int pivot_idx, val* nums);
public:
	val findKthLargestAndPivot();
	QMaxKV(int q, float gamma);
	~QMaxKV();
	void insert(key k, val v);
	outputkv largestQ();
	void update(key k, val v);
	val getMinimalVal();
	key getMinimalKey();
};

#endif

