#include "Qmax.hpp"
#include "SlidingQmax.hpp"
#include <iostream>

void SlidingQMax::print(){
	for (int i = 0; i < _nrQmaxs; ++i)
		_Q[i]->print();
}

SlidingQMax::SlidingQMax(int W, int q, float gamma, float tau){
	_nrQmaxs = 1/tau + 1;
	_blockSize = W*tau;
	_curOffset = _blockSize;
	_Q = new QMax*[_nrQmaxs];
	for (int i = 0; i < _nrQmaxs; ++i){
		_Q[i] = new QMax(q, gamma);
		if (!_Q[i])
			exit(1);
	}
	_curQMax = _Q[0];
	_nextQMaxIdx = _nrQmaxs;
	_gamma = gamma;
	_q = q;
} 
void SlidingQMax::insert(int v){
	_curQMax->insert(v);
	if (--_curOffset)
		return;
	_curOffset = _blockSize;
	_curQMax = _Q[--_nextQMaxIdx];
	_curQMax->reset();	
	if (_nextQMaxIdx)
		return;
	_nextQMaxIdx = _nrQmaxs;
}

int* SlidingQMax::largestQ(){
	return NULL;
}

