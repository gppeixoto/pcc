#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <map>

#define ALPHABET_SIZE 140

using namespace std;

typedef pair<string, int> psi; 

class Node {
    public:
        static int num;
        int id;
        map<char, Node*> go_to;
        Node* fail;
        vector<string> occ;
        Node() : go_to(), fail(), occ(ALPHABET_SIZE), id(num){
            num += 1;
        };
};

int Node::num = 0;

typedef pair<char, Node*> pcn;
typedef map<char, Node*> map_pcn;

Node* build_fail(Node* start){
    queue<Node*> q;
    Node* curr;
    Node* next;
    Node* brd;
    for (int i=0; i < ALPHABET_SIZE; ++i){
        if (start->go_to[i]->id != start->id){
            q.push(start->go_to[i]);
            start->go_to[i]->fail = start;
        }
    }
    while (!q.empty()){
        curr = q.front(); q.pop();
        map<char, Node*>::iterator it;
        for (it = curr->go_to.begin(); it != curr->go_to.end(); ++it){
            next = it->second; 
            q.push(next);
            brd = curr->fail;
            char c = it->first;
            map<char, Node*>::iterator it = brd->go_to.find(c);
            
            while (it == brd->go_to.end()){
                brd = brd->fail;
            }
            next->fail = brd->go_to[c];
            next->occ.insert(next->occ.end(), next->fail->occ.begin(), next->fail->occ.end());
        }
    }
    return start;
}

Node* build_goto(Node* start, vector<string> patterns){
    Node* curr;
    Node* new_node;
    int j;

    for (int i=0; i < patterns.size(); ++i){
        curr = start;
        j = 0;
        string pat = patterns[i];
        map<char, Node*>::iterator it;  //looks for pat_0 in the
        it = curr->go_to.find(pat[j]);  //adjacent nodes of start
        while (j < pat.length() && it != curr->go_to.end()) {
            curr = curr->go_to[pat[j]];
            j += 1;
            it = curr->go_to.find(pat[j]);
        }

        while (j < pat.length()){
            new_node = new Node();
            curr->go_to[pat[j]] = new_node;
            curr = new_node;
            j += 1;
        }

        curr->occ.push_back(pat);
        for (int i=0; i < ALPHABET_SIZE; ++i){
            char tmp = i;
            map<char, Node*>::iterator it; it = start->go_to.find(tmp);
            if (it == start->go_to.end()){ //if didnt find char i
                start->go_to[tmp] = start;
            }
        }
    }
    return start;
}

Node* build_fsm(vector<string> patterns){
    Node* start = new Node();
    start = build_goto(start, patterns);
    start = build_fail(start);
    return start;
}

vector<psi> aho_corasick(string text, vector<string> patterns){
    Node* fsm = build_fsm(patterns);
    Node* curr = fsm;
    int len = text.length();
    vector<psi> occurences;
    map_pcn::iterator it;
    
    for (int i=0; i<len; ++i){
        it = curr->go_to.find(text[i]);
        while (it == curr->go_to.end()){
            curr = curr->fail;
            it = curr->go_to.find(text[i]);
        }
        curr = curr->go_to[text[i]];
        for (int j=0; j<curr->occ.size(); ++j){
            string pat = curr->occ[i];
            occurences.push_back(psi(pat, i-pat.length()+1));
        }
    }
    return occurences;
}

int main() {
    vector<string> pats;
    pats.push_back("she"); pats.push_back("hers"); pats.push_back("he"); pats.push_back("ushers");
    Node* start = build_fsm(pats);
    string s = "he lost hers keys";
    vector<psi> occ = aho_corasick(s, pats);

    return 0;
}
