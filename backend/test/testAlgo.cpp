/*
* Test de compilation
*/

#include "FA.h"
#include "TA.h"
#include <time.h>


using namespace std;

bool dsc(const FA::TF &a,const FA::TF &b){
       return ((a.frequency > b.frequency) ||(a.frequency == b.frequency && a.d<b.d));
}
bool dsc1(const TA::TF &a,const TA::TF &b){
       return ((a.frequency > b.frequency) ||(a.frequency == b.frequency && a.d<b.d));
}
vector<vector<FA::TF>> makeTab1(int x,int y){
	vector<vector<FA::TF>> tab;
	vector<FA::TF> vtf;
	srand(time(NULL));
	for(int i=0;i<x;i++){
		for(int j=1;j<=y;j++){
			vtf.push_back({j,(double) rand() / (RAND_MAX)});
		}
		sort(vtf.begin(), vtf.end(), dsc);
		tab.push_back(vtf);
		vtf.clear();
	}
	return tab;
}
vector<vector<TA::TF>> makeTab2(int x,int y){
	vector<vector<TA::TF>> tab;
	vector<TA::TF> vtf;
	srand(time(NULL));
	for(int i=0;i<x;i++){
		for(int j=1;j<=y;j++){
			vtf.push_back({j,(double) rand() / (RAND_MAX)});
		}
		sort(vtf.begin(), vtf.end(), dsc1);
		tab.push_back(vtf);
		vtf.clear();
	}
	return tab;
}
void testFA(FA fa,vector<vector<FA::TF>> t,int k){
	clock_t start,end;
	start=clock();
	fa.FAlgo(k,t);
	fa.displayC();
	end=clock();
	cerr<<"time:"<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
}
void testTA(TA ta,vector<vector<TA::TF>> t,int k){
	clock_t start,end;
	start=clock();
	ta.TAlgo(k,t);
	ta.displayC();
	end=clock();
	cerr<<"time:"<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
}
int main_old() {
	vector<vector<FA::TF>> tab1;
	//tab1=makeTab1(20,1500);
	vector<vector<TA::TF>> tab2;
	//tab2=makeTab2(20,1500);
	tab1={{{1,1},{2,0.8},{3,0.5},{4,0.3},{5,0.1}},{{2,0.8},{3,0.7},{1,0.3},{4,0.2},{5,0.1}},{{4,0.8},{3,0.6},{1,0.2},{5,0.1},{2,0}}};
	tab2={{{1,1},{2,0.8},{3,0.5},{4,0.3},{5,0.1}},{{2,0.8},{3,0.7},{1,0.3},{4,0.2},{5,0.1}},{{4,0.8},{3,0.6},{1,0.2},{5,0.1},{2,0}}};
	int k=3;
	FA fa;
	TA ta;
	testFA(fa,tab1,k);
	testTA(ta,tab2,k);
	return 0;
}
