#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <deque>
#include <numeric>
using namespace std;

#include "FA.h"

class TA{
public:
	using TF = ::TF;
	using TS = ::TS;
	vector<TS> C;
	vector<TF> M;
	double t;
	vector<TS> result;
	double calculScore(TF pl,vector<vector<TF>> &tab);
	void InsertC(TF pl,vector<vector<TF>> &tab);
	void removeC(int k);
	void sortedAccess(int row,vector<vector<TF>> &tab);
	bool kDocT(int k);
	void display_vector(const vector<int> &v);
	void displayC();
	void displayTab(vector<vector<TF>> &tab);
	void step1(int k,vector<vector<TF>> &tab);
	void step2(int k);
	void TAlgo(int k,vector<vector<TF>> &tab);
};
