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

	v_psi aho_corasick(const string &text) {
	    int cur = START;
	    v_psi occs;
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
	            occs.push_back(psi(*it, i-(*it).length()+1));
	            ++it;
	        }
	    }
	    return occs;
	}

	void print_results(const v_psi &occs, const string &text) {
	    // for (unsigned long i=0; i < text.length(); ++i)
	    // {
	    //     string prt = ((i/10)>0 && i%10==0) ? to_string(char(i/10)) : " ";
	    //     cout << prt;
	    // }
	    // cout << endl;

	    // for (unsigned long i=0; i < text.length(); ++i)
	    //     cout << i%10;
	    
	    // cout << endl;
	    // cout << text << endl << endl;
	    if (occs.size() > 0) for (psi pat_pos : occs)
	    {
	    	cout << "### MATCH ###" << endl;
	    	cout << text << endl;
	        cout << "\t" << pat_pos.first << " " << pat_pos.second << endl;
	        cout << "### MATCH ###" << endl << endl;
	    }
	}
};


void run_aho(string &txtfile, vs &patterns){
	ifstream mystream(txtfile);
	if (!mystream.good())
	{
		cout << "Arquivo de texto " << txtfile << " nao existe" << endl;
	}

	string line;
	AhoCorasick aho = AhoCorasick();
	aho.build_fsm(patterns);
	while(getline(mystream, line)){
		v_psi v = aho.aho_corasick(line);
		aho.print_results(v, line);
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