/*Source: 	Ronald Fagin. Combining fuzzy information from multiple systems.
*/

/*
Fagin’s top-k query algorithm (FA)
1. M = ∅; C = ∅; (Step 0)
2. Repeat until |C|=k (Step 1)
	1. Sorted access in parallel to the qt (let d be the doc met)
	2. If d has been seen for all the qt
		1. Remove d from M
		2. Insert (d,s(t1+t2+…, d)) into C
	3. Else if d is seen for the first time
		1. Insert d into M
3. For each d ∈ M (Step 2)
	1. Random access to all remaining qt to compute the aggregated score of d
	2. Insert (d,s(t1+t2+…, d)) into C
4. Return the top-k docs in C (Step 3)

*/
#include "FA.h"
using namespace std;
inline bool sortinrev(const FA::TS &a,const FA::TS &b){
       return ((a.score > b.score) ||(a.score == b.score && a.d<b.d));
}

void FA::removeDoc(TF tf) {
	vector<int>::iterator it=find(M.begin(),M.end(),tf.d);
	if(it!=M.end()){
		M.erase(it);
	}
}

bool FA::hasSeenForAll(TF tf,vector<vector<int>>& qt){
	int docID=tf.d;
	for ( int i=0;i<(int)qt.size();i++ ){
	   for ( int j=0;j<(int)qt.at(0).size();j++ ){
		   if(find(qt.at(i).begin(),qt.at(i).end(),docID)==qt.at(i).end()){
			   return false;
		   }
	   }
	}
	return true;
}

bool FA::SeenFirstTime(TF tf){
	if(find(M.begin(),M.end(),tf.d)!=M.end()){
		return false;
	}
	else{
		return true;
	}
}
double FA::calculScore(int docID,vector<vector<TF>>& tab){
	double sum = 0.0;
	for ( const auto &row : tab ){
	   for ( const auto &s : row ){
		   if(s.d==docID){
			   sum += s.frequency;
		   }
	   }
	}
	return sum/tab.size();
}
void FA::display_vector(const vector<int> &v)
{
    std::copy(v.begin(), v.end(),
        std::ostream_iterator<int>(std::cout, " "));
    cout<<endl;
}
void FA::displayC()
{
	 for(int i=0;i<(int)C.size();i++){
	    	cout<<C.at(i).d<<" "<<C.at(i).score<<"; ";
	 }
	 cout<<endl;
}

void FA::displayTab(vector<vector<TF>> &tab){
    for(vector<TF> vtf: tab){
    	for(TF tf: vtf){
    		cout<<tf.d<<" "<<tf.frequency<<"; ";
    	}
    }
    cout<<endl;
}
void FA::step1(int k,vector<vector<TF>>& tab){
	int row=0;
	vector<vector<int>> qt(tab.size());
	vector<int> v;
	C.clear();
	if(k>tab.at(0).size()){
		for (TF tf1:tab.at(0)){
			C.push_back({tf1.d,calculScore(tf1.d,tab)});
		}
	}
	else{
		TF tf;
		while((int)C.size()!=k && row <tab.at(0).size() ){
			for (int i=0;i<(int)tab.size();i++){
				if(tab.at(i).size()>row){
					tf=tab.at(i).at(row);
					qt.at(i).push_back(tf.d);
					if(hasSeenForAll(tf,qt)){
						removeDoc(tf);
						C.push_back({tf.d,calculScore(tf.d,tab)});
					}
					else if(SeenFirstTime(tf)){
						M.push_back(tf.d);
					}
				}
			}
			row++;
		}
	}
}

void FA::step2(vector<vector<TF>>& tab){
	for(auto d : M){
		C.push_back({d,calculScore(d,tab)});
	}
	M.clear();
}

void FA::step3(int k){
	result.clear();
	sort(C.begin(), C.end(), sortinrev);
	for(int i=0;i<k;i++){
		if(i<C.size()){
			result.push_back(C.at(i));
		}
	}
	C.clear();
	C=result;
}

vector<FA::TS> FA::FAlgo(int k,vector<vector<TF>>& tab){
	step1(k,tab);
	step2(tab);
	step3(k);
	//return result;
	return C;
}
