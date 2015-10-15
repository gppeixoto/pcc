#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <map>
#include <set>

#define FAIL 0
#define START 1
#define ALPHABET_SIZE 26
#define MAX_S 30
#define DEBUG cout << "DEBUG" << endl

using namespace std;

typedef std::vector<std::string> vs;
typedef std::vector<int> vi;

int fail[MAX_S];
int go_to[MAX_S][ALPHABET_SIZE];
set<int> occ[MAX_S];

void print_goto()
{
    cout << "  ";
    for (int i=0; i < ALPHABET_SIZE; ++i) {
        char tmp = i+'a';
        cout << tmp << " ";
    }
    cout << endl;
    for (int i=0; i < MAX_S; ++i) {
        cout << i%10 << " ";
        for (int j=0; j < ALPHABET_SIZE; ++j) {
            cout << go_to[i][j] << " ";
        }
        cout << endl;
    }
}

void print_fail()
{
    cout << "--FAIL--" << endl;
    for (int i=0; i < MAX_S; ++i) {
        cout << i << "\t" << fail[i] << endl;
    }
    cout << "--FAIL--" << endl;
}

void build_goto(const vs &patterns) {
    memset(go_to, 0, sizeof go_to);
    
    int curr = START;
    int state = 2;
    for (int i=0; i < patterns.size(); ++i) {
        curr = START;
        string pat = patterns[i];
        for (int j=0; j < pat.size(); ++j) {
            int key = pat[j]-'a';
            if (go_to[curr][key] == FAIL) {
                go_to[curr][key] = state;
                state += 1;
            }
            curr = go_to[curr][key];
        }
        occ[curr].insert(i+1);
    }
}

void build_fail(const vs &patterns) {

    queue<int> q;
    for (int a=0; a < ALPHABET_SIZE; ++a) {
        int s = go_to[START][a];
        if (s != FAIL) {
            q.push(s);
            fail[s] = FAIL;
        }
    }

    while (!q.empty()) {
        int r = q.front(); q.pop();
        for (int a=0; a < ALPHABET_SIZE; ++a) {
            int s = go_to[r][a];
            int u = r;
            if (s != FAIL) {
                q.push(s);
                int state = fail[r];
                while (u != FAIL && go_to[state][a] == FAIL)
                    u = fail[u];
                fail[u] = (u == START) ? START : go_to[fail[u]][a];
                occ[s].insert(occ[fail[s]].begin(), occ[fail[s]].end());
            }
        }
    }
}

void build_fsm(const vs &patterns) {
    build_goto(patterns);
    build_fail(patterns);
}

void aho_corasick(const vs &patterns, const string &text){
    int state = START;
    for (int i=0; i < text.length(); ++i) {
        int a_i = text[i]-'a';
        while (go_to[state][a_i] == FAIL && state != FAIL)
            state = fail[state];
        state = (state == FAIL) ? START : go_to[state][a_i];
        if (!occ[state].empty()) {
            for (set<int>::iterator it = occ[state].begin();
                    it != occ[state].end(); ++it)
            {
                int o = patterns[(*it)-1].length();
                cout << i-o+1 << " " << patterns[(*it)-1] << endl;
            }
        }
    }
}

int main() {
    vs pats;
    pats.push_back("she"); pats.push_back("som"); pats.push_back("he"); 
    pats.push_back("hers");
    build_fsm(pats);
    string text = "guilhermepalmapeixoto";
    aho_corasick(pats, text);
    return 0;
}
