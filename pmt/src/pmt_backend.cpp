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