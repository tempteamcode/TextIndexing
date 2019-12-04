#pragma once

#include <vector>

#include "search.h"
#include "InvertedFile.h"

using std::vector;

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
	void step1(unsigned int k,vector<vector<TF>> &tab);
	void step2(vector<vector<TF>> &tab);
	void step3(unsigned int k);
	vector<FA::TS> FAlgo(int k,vector<vector<TF>> &tab);
};

inline SearchResults_t resultsFA_to_SearchResults_t(const vector<FA::TS>& resultsFA_not_as_SearchResults_t)
{
	SearchResults_t resultsFA;
	for (const auto& res : resultsFA_not_as_SearchResults_t)
	{
		DocFreq_t freq;
		freq.docID = res.d;
		freq.frequency.first = static_cast<unsigned int>(res.score * 50000);
		freq.frequency.second = 50000;
		resultsFA.push_back(freq);
	}

	return resultsFA;
}

inline std::vector<std::vector<FA::TF>> IF_sort(const InvertedFile_t& IF)
{
	std::vector<std::vector<FA::TF>> tab;
	for (const auto& it : IF)
	{
		const auto& vec = it.second;
		std::vector<FA::TF> row; row.reserve(vec.size());
		for (const auto& elem : vec)
		{
			FA::TF item;
			item.d = elem.first;
			item.frequency = freqTF(elem.second);
			row.push_back(item);
		}
		std::sort(row.begin(), row.end(), [](FA::TF& f1, FA::TF& f2) {
			return (f1.frequency > f2.frequency) || (f1.frequency == f2.frequency && f1.d < f2.d);
		});
		tab.push_back(row);
	}

	return tab;
}
