#ifndef QMAXPBAKV_H
#define QMAXPBAKV_H
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unordered_map>

using namespace std;

typedef unsigned long long weight;

typedef struct pba_val {
	weight w;
	double a;
	double q;
	double u;
	double w_by_u;
} pba_val;

typedef unsigned long long key;
typedef pba_val val;

typedef struct output {
	key *keyArr;
	val *valArr;
} outputkv;

class QMaxPBAKV
{
        key *_K;
	val *_V;
	unordered_map<key, unsigned int> hash_table;
	unsigned int _curIdx;
	int _q;
	int _actualsize;
	int _actualsizeMinusOne;
	int _qMinusOne;
	float _gamma;
	int _nminusq;
	weight _phi;
	weight z_star;
	void maintenance();
	inline void swap(int a, int b);
	int PartitionAroundPivot(int left, int right, int pivot_idx, val* nums);
public:
	weight findKthLargestAndPivot();
	QMaxPBAKV(int q, float gamma);
	~QMaxPBAKV();
	void insert(key k, weight x, double u);
	outputkv largestQ();
	bool in(key k);
};

#endif

