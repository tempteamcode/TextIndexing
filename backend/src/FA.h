/**
 * @file FA.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief 
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <deque>
using namespace std;

struct TF {
	int d;
	double frequency;
};
struct TS {
	int d;
	double score;
};

class FA{
public:
	using TF = ::TF;
	using TS = ::TS;
	vector<int> M;
	vector<TS> C;
	vector<TS> result;
	void removeDoc(TF tf);
	bool hasSeenForAll(TF tf,vector<vector<int>> &qt);
	bool SeenFirstTime(TF tf);
	double calculScore(int docID,vector<vector<TF>> &tab);
	void display_vector(const vector<int> &v);
	void displayC();
	void displayTab(vector<vector<TF>> &tab);
	void step1(int k,vector<vector<TF>> &tab);
	void step2(vector<vector<TF>> &tab);
	void step3(int k);
	vector<FA::TS> FAlgo(int k,vector<vector<TF>> &tab);
};
