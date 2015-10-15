#include "pmt_backend.h"

struct Kmp
{
	int n, m;
	vector<int> back;
	vector<int> occ;

	//preprocess 
	void preprocess(string &pat){
		m = pat.length();
		back.resize(m+1);//sets length of vector to text size
		int i = 0, j = -1;
		back[0] = -1;
		while(i<m){
			while(j>=0 && (pat[i] != pat[j])){
				j = back[j];
			}
			i++;
			j++;
			back[i] = j;
		}
	}

	void kmp(string &txt, string &pat){
		occ.clear();
		n = txt.length();
		int i = 0, j = 0;
		while(i<n){
			while(j>=0 && txt[i] != pat[j]){
				j = back[j];
			}
			i++;
			j++;
			if(j==m){
				occ.push_back(i-j);
				j = back[j];
			}
		}
	}

};

void run_kmp(string &txtfile, string &pattern){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	Kmp kmp = Kmp();
	kmp.preprocess(pattern);
	while(getline(mystream, line)){
		kmp.kmp(line, pattern);
		for(int &pos : kmp.occ){
			cout << line << ":" << pos << endl;
		}
	}
}


struct Sellers
{
	int m, n;

	void sellers(string &txt, string &pat, int maxError){
		n = txt.length();
		m = pat.length();

		vector<vector<int>> distance;		

		//keeps only current and previous
		for(int i = 0; i <= m+1; i++){
			distance.push_back( vector<int>( 2 ));
		}
		
		for(int i =0; i <=m; i++) 
			distance[i][0] = i;
		for(int j =0; j < 2; j++)
			distance[0][j] = 0;

		for(int j =1; j <=n; j++){
		
			int cur_j = j % 2; // current
			int ant_j = cur_j == 0? 1 : 0; // previous
			
			for(int i = 1; i<=m; i++){
				distance[i][cur_j] = min(distance[i-1][ant_j] + ((pat[i-1] == txt[j-1])? 0 : 1), 
					min(distance[i][ant_j] + 1, distance[i-1][cur_j] + 1));//sellers algorithm
			}
			if(distance[m][cur_j] <= maxError){//found
				cout << "found " << pat << " in " << txt << endl;
				break;//only prints line
			}
		}
	}
};

void run_sellers(string &txtfile, string &pattern, int maxError){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	Sellers sellers = Sellers();
	while(getline(mystream, line)){
		sellers.sellers(line, pattern, maxError);
	}
}