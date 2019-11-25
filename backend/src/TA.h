#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <deque>
#include <numeric>
using namespace std;

class TA{
	public:
	struct TF{
			int d;
			double frequency;
		};
		struct TS{
			int d;
			double score;
		};
	vector<TS> C;
	vector<TF> M;
	double t;
	vector<TS> result;
	double scoreTotal(TF pl,vector<vector<TF>> &tab);
	void InsertC(TF pl,vector<vector<TF>> &tab);
	void removeC(int k);
	void sortedAccess(int row,vector<vector<TF>> &tab);
	bool kDocT(int k);
	void display_vector(const vector<int> &v);
	void displayC();
	void displayTab(vector<vector<TF>> &tab);
	void step1(int k,vector<vector<TF>> &tab);
	void step2(int k);
	vector<TS> TAlgo(int k,vector<vector<TF>> &tab);
};
