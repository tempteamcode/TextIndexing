#pragma once

#include <vector>

#include "search.h"

using std::vector;

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
	void step1(unsigned int k,vector<vector<TF>> &tab);
	void step2(unsigned int k);
	vector<TA::TS> TAlgo(int k,vector<vector<TF>> &tab);
};

inline SearchResults_t resultsTA_to_SearchResults_t(const vector<TA::TS>& resultsTA_not_as_SearchResults_t)
{
	SearchResults_t resultsTA;
	for (const auto& res : resultsTA_not_as_SearchResults_t)
	{
		DocFreq_t freq;
		freq.docID = res.d;
		freq.frequency.first = static_cast<unsigned int>(res.score * 50000);
		freq.frequency.second = 50000;
		resultsTA.push_back(freq);
	}

	return resultsTA;
}
