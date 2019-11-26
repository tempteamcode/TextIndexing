#include "TA.h"
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
double TA::scoreTotal(TF pl,vector<vector<TF>> &tab){
	double sum = 0.0;
	for ( const auto &row : tab ){
	   for ( const auto &s : row ){
		   if(s.d==pl.d){
			   sum += s.frequency;
		   }
	   }
	}
	return sum;
}
void TA::InsertC(TF pl, vector<vector<TF>> &tab) {
	bool flag=true;
	double score;
	for(TS itPL:C){
		if(itPL.d==pl.d){
			flag=false;
		}
	}
	if(flag){
		score=scoreTotal(pl,tab);
		TS TS={pl.d,score};
		C.push_back(TS);
	}
}
void TA::display_vector(const vector<int> &v){
    std::copy(v.begin(), v.end(),
        std::ostream_iterator<int>(std::cout, " "));
    cout<<endl;
}
void TA::displayC(){
    for(int i=0;i<(int)C.size();i++){
    	cout<<C.at(i).d<<" "<<C.at(i).score<<"; ";
    }
    cout<<endl;
}

void TA::displayTab(vector<vector<TF>> &tab){
    for(vector<TF> vpl: tab){
    	for(TF pl: vpl){
    		cout<<pl.d<<" "<<pl.frequency<<"; ";
    	}
    }
    cout<<endl;
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
}
/*
 * At each sequential access
	(a) Set the threshold t to be the aggregate of the scores seen in this access.
	(b) Do random accesses and compute the scores of the seen objects.
	(c) Maintain a list of top-k objects seen so far
	(d) Stop, when the scores of the top-k are greater or equal to the threshold.

 * */
void TA::step1(int k,vector<vector<TF>> &tab){
	int row=0;
	//vector<vector<int>> qt(tab.size());
	vector<int> v;
	TF pl;
	vector<double> vTau(tab.size());
	while(kDocT(k)){
		sortedAccess(row,tab);
		for (vector<TF>& qt: tab){
			pl=qt.at(row);
			InsertC(pl,tab);
		}
		removeC(k);
		row++;
	}
}
/*
 * 2. Return the top-k seen so far
 * */
void TA::step2(int k){
	result.clear();
	sort(C.begin(), C.end(), sortinrev);
	for(int i=0;i<k;i++){
		result.push_back(C.at(i));
	}
}

vector<TA::TS> TA::TAlgo(int k,vector<vector<TF>> &tab){
	step1(k,tab);
	step2(k);
	return result;
}
