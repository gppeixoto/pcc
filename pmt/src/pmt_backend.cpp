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

	void kmp(string &txt, string &pat, bool silent){
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
				if(!silent) cout << txt << endl;
				return;
				// occ.push_back(i-j);
				// j = back[j];
			}
		}
	}

};

void run_kmp(string &txtfile, string &pattern, bool silent){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	Kmp kmp = Kmp();
	kmp.preprocess(pattern);
	while(getline(mystream, line)){
		kmp.kmp(line, pattern, silent);
	}
}

struct AhoCorasick
{
	vector<int> fail; //[max_st]
	vector<vector<int>> go_to; //[max_st][size]
	vector<set<string>> occ; //[max_st]

	void build_goto(const vs &patterns) {
	    //memset(go_to, FAIL, sizeof go_to);
	    for (unsigned long i=0; i < go_to.size(); ++i) {
	    	go_to[i].resize(SIZE);
	    	for (unsigned long j = 0; j < go_to[i].size(); ++j) {
	    		go_to[i][j] = FAIL;
	    	}
	    }

	    int newstate = START;
	    for (string pat : patterns)
	    {
	        int state = START;
	        unsigned long j = 0;
	        int pat_j = pat[j] - MIN_CHAR;

	        while (j < pat.length() && go_to[state][pat_j] != FAIL)
	        {
	            state = go_to[state][pat_j];
	            j += 1;
	            pat_j = pat[j] - MIN_CHAR;
	        }

	        while (j < pat.length())
	        {
	            newstate += 1;
	            pat_j = pat[j] - MIN_CHAR;
	            go_to[state][pat_j] = newstate;
	            state = newstate;
	            j += 1;
	        }
	        occ[state].insert(pat);
	    }

	    for (int a=0; a < SIZE; ++a)
	        if (go_to[START][a] == FAIL)
	            go_to[START][a] = START;
	}

	void build_fail() {
	    queue<int> q;
	    for (int a=0; a < SIZE; ++a)
	    {
	        if (go_to[START][a] != START) {
	            q.push(go_to[START][a]);
	            fail[go_to[START][a]] = START;
	        }
	    }

	    while (!q.empty())
	    {
	        int cur = q.front(); q.pop();
	        for (int a=0; a < SIZE; ++a)
	        {
	            if (go_to[cur][a] != FAIL) {
	                int nextnode = go_to[cur][a];
	                q.push(go_to[cur][a]);
	                int brd = fail[cur];
	                while (go_to[brd][a] == FAIL)
	                    brd = fail[brd];
	                fail[nextnode] = go_to[brd][a];
	                set<string>::iterator it = occ[fail[nextnode]].begin();
	                while (it != occ[fail[nextnode]].end())
	                {
	                    occ[nextnode].insert(*it);
	                    ++it;
	                }
	            }
	        }
	    }
	}

	void build_fsm(const vs &patterns) {
		int sz = 0;
		for (auto pat : patterns) {
			sz += pat.size();
		} sz += 50;
		fail.resize(sz);
		go_to.resize(sz);
		for (auto vec : go_to) {
			vec.resize(SIZE);
		}
		occ.resize(sz);
	    build_goto(patterns);
	    build_fail();
	}

	void aho_corasick(const string &text, bool silent) {
	    int cur = START;
	    for (unsigned long i=0; i < text.length(); ++i)
	    {
	        int a = text[i] - MIN_CHAR;
	        while (go_to[cur][a] == FAIL){
	            cur = fail[cur];
	        }
	        cur = go_to[cur][a];
	        set<string>::iterator it = occ[cur].begin();
	        while (it != occ[cur].end())
	        {
	            if(!silent) cout << text << endl;
	            return;
	        }
	    }
	}
};


void run_aho(string &txtfile, vs &patterns, bool silent){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	AhoCorasick aho = AhoCorasick();
	aho.build_fsm(patterns);
	while(getline(mystream, line)){
		aho.aho_corasick(line, silent);
	}
}

struct Sellers
{
	int m, n;

	void sellers(string &txt, string &pat, int maxError, bool silent){
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
				if(!silent)cout << txt << endl;
				break;//only prints line
			}
		}
	}
};

void run_sellers(string &txtfile, string &pattern, int maxError, bool silent){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	Sellers sellers = Sellers();
	while(getline(mystream, line)){
		sellers.sellers(line, pattern, maxError, silent);
	}
}

struct WuManber
{
	ull char_mask[ALPHABET_SIZE];

	void initCharMask(string &pat){
		int m = pat.size();
		for (int i = 0; i < ALPHABET_SIZE; ++i){
			char_mask[i] = ~0;
		}
		for (int i = 0; i < m; ++i){
			char_mask[(int)pat[i]] &= ~(1 << i);//clear ith bit
		}
	}

	void wuManber(string &txt, string &pat, int err, bool silent){
		int n = txt.size();
		int m = pat.size();
		//generate S
		ull S[err+1];
		for (int i = 0; i <= err; ++i){
			S[i] = (~0) << i;
		}
		for (int j = 0; j < n; ++j){
			ull msk = char_mask[(int)txt[j]];
			ull old = S[0],tmp;
			S[0] |= msk;
			S[0] <<= 1;//shift or
			for (int q = 1; q <= err; ++q){
				tmp = S[q];
				//S[q] = (old & (S[q] | msk)) << 1;
				S[q] = 	((S[q] | msk) << 1)
						& (old << 1)
						& (S[q-1] << 1)
						& old;
				old = tmp;
				if(!(S[err] & (1UL << m))){//most significant bit is 0 
					//found
					if(!silent) cout << txt << endl;
					return;
				}
			}
		}
	}

};

void run_wu_manber(string &txtfile, string &pattern, int maxError, bool silent){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	WuManber wu = WuManber();
	wu.initCharMask(pattern);
	while(getline(mystream, line)){
		wu.wuManber(line, pattern, maxError, silent);
	}
}