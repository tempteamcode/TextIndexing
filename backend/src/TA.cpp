/*Source: 	Combining Fuzzy Information: An Overview. Ronald Fagin.
		A Survey of Top-k Query Processing Techniques in Relational Database Systems. Ilyas, Beskales and Soliman.
		http://alumni.cs.ucr.edu/~skulhari/Top-k-Query.pdf
*/

/*
 * Threshold Algorithm
 	1. Access the elements sequentially and at each sequential access (Step 1)
	(a) Set the threshold t to be the aggregate of the scores seen in this access.
	(b) Do random accesses and compute the scores of the seen objects.
	(c) Maintain a list of top-k objects seen so far
	(d) Stop, when the scores of the top-k are greater or equal to the threshold.
	2. Return the top-k seen so far (Step 2)
 * */

#include "utility.h"

#include "TA.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator>
#include <deque>
#include <numeric>
using namespace std;

inline bool sortinrev(const TA::TS &a,const TA::TS &b){
	return ((a.score > b.score) ||(a.score == b.score && a.d<b.d));
}
void TA::removeC(int k) {
	sort(C.begin(),C.end(),sortinrev);
	while((int)C.size()>k){
		C.erase(C.end()-1);
	}
}
double TA::calculScore(TF pl,vector<vector<TF>> &tab){
	double sum = 0.0;
	for ( const auto &row : tab ){
	   for ( const auto &s : row ){
		   if(s.d==pl.d){
			   sum += s.frequency;
		   }
	   }
	}
	return sum/tab.size();
}
void TA::InsertC(TF pl, vector<vector<TF>> &tab) {
	bool flag=true;
	for(TS itPL:C){
		if(itPL.d==pl.d){
			flag=false;
		}
	}
	if(flag){
		TS TS={pl.d,calculScore(pl,tab)};
		C.push_back(TS);
	}
}
void TA::display_vector(const vector<int> &v){
	if (outputOnlyJSON) return;
    std::copy(v.begin(), v.end(),
        std::ostream_iterator<int>(std::cout, " "));
    std::cout<<std::endl;
}
void TA::displayC(){
	if (outputOnlyJSON) return;
    for(int i=0;i<(int)C.size();i++){
    	std::cout<<C.at(i).d<<" "<<C.at(i).score<<"; ";
    }
    std::cout<<std::endl;
}
void TA::displayTab(vector<vector<TF>> &tab){
	if (outputOnlyJSON) return;
    for(vector<TF> vpl: tab){
    	for(TF pl: vpl){
    		std::cout<<pl.d<<" "<<pl.frequency<<"; ";
    	}
    }
    std::cout<<std::endl;
}

bool TA::kDocT(int k){
	int i=0;
	for(TS TS: C){
		if(TS.score>=t){
			i++;
		}
		if (i>=k) return false;
	}
	return true;
}
void TA::sortedAccess(int row,vector<vector<TF>> &tab){
	t=0;
	for(vector<TF> vpl:tab){
		t=t+vpl.at(row).frequency;
	}
	t=t/tab.size();
}

void TA::step1(unsigned int k,vector<vector<TF>> &tab){
	int row=0;
	if (k>tab.at(0).size()){
		for (TF tf1:tab.at(0)){
			C.push_back({tf1.d,calculScore(tf1,tab)});
		}
	}
	else{
		while(kDocT(k)){
			TF pl;
			sortedAccess(row,tab);
			for (vector<TF> qt: tab){
				pl=qt.at(row);
				InsertC(pl,tab);
			}
			removeC(k);
			row++;
		}
	}
}
void TA::step2(unsigned int k){
	result.clear();
	sort(C.begin(), C.end(), sortinrev);
	for(unsigned int i=0;i<k;i++){
		if(i<C.size()){
			result.push_back(C.at(i));
		}
	}
	C.clear();
	C=result;
}

vector<TA::TS> TA::TAlgo(int k,vector<vector<TF>> &tab){
	step1(k,tab);
	step2(k);
	//return result;
	return C;
}
