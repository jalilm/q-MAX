#ifndef SLIDINGQMAX_H
#define SLIDINGQMAX_H
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Qmax.hpp"
class SlidingQMax
{
	QMax **_Q; 
	int _nrQmaxs;
	int _blockSize;
	int _curOffset;
	QMax *_curQMax;
	int _nextQMaxIdx;
	float _gamma;
	int _q;
public:
	SlidingQMax(int W, int q, float gamma, float tau);
	void insert(int id);
	int* largestQ();
	void print();
};
#endif

