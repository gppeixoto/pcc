#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <queue>
#include <set>

#define FAIL 0
#define START 1
#define MIN_CHAR 32
#define MAX_CHAR 126
#define SIZE MAX_CHAR - MIN_CHAR
#define MAX_ST 30

using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;

int fail[MAX_ST];
int go_to[MAX_ST][SIZE];
set<string> occ[MAX_ST];

void build_goto(const vs &patterns) {
    memset(go_to, FAIL, sizeof go_to);
    int newstate = START;
    for (string pat : patterns)
    {
        int state = START;
        int j = 0;
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

void build_fail(const vs &patterns) {
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
    build_goto(patterns);
    build_fail(patterns);
}

v_psi aho_corasick(const vs &patterns, const string &text) {
    int cur = START;
    v_psi occs;
    for (int i=0; i < text.length(); ++i)
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
    for (int i=0; i < text.length(); ++i)
    {
        string prt = ((i/10)>0 && i%10==0) ? to_string(char(i/10)) : " ";
        cout << prt;
    }
    cout << endl;

    for (int i=0; i < text.length(); ++i)
        cout << i%10;
    
    cout << endl;
    cout << text << endl << endl;
    for (psi pat_pos : occs)
    {
        cout << pat_pos.first << " " << pat_pos.second << endl;
    }
}

int main() {
    vs pats;
    pats.push_back("sit");
    pats.push_back("sect");
    pats.push_back("...");
    pats.push_back("dolor");
    pats.push_back(" dolor ");
    pats.push_back("r s");
    pats.push_back(",");
    build_fsm(pats);

    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit...";
    v_psi occs = aho_corasick(pats, text);

    print_results(occs, text);
    return 0;
}






