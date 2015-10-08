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

void build_fail(Node* start){
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
            next->occ.push(next->fail);
            //v.reserve(v.size() + distance(v_prime.begin(),v_prime.end()));
            //v.insert(v.end(),v_prime.begin(),v_prime.end());
            //nextnode.occ.extend(nextnode.fail.occ)
        }
    }
}

Node* build_goto(Node* start, vector<string> patterns){
    for (int i=0; i < patterns.size(); ++i){
        Node* curr = start;
        int j = 0;
        string pat = patterns[i];
        map<char, Node*>::iterator it; it = curr->go_to.find(pat[j]);
       
        while (j < pat.length() && it != curr->go_to.end()) {
            curr = curr->go_to[pat[j]];
            j += 1;
        }

        while (j < pat.length()){
            Node* new_node = new Node();
            curr->go_to[pat[j]] = new_node;
            curr = new_node;
            j += 1;
        }
        curr->occ.push_back(pat);
        for (int i=0; i < ALPHABET_SIZE; ++i){
            char tmp = i;
            map<char, Node*>::iterator it; it = start->go_to.find(tmp);
            if (it == start->go_to.end()){
                start->go_to[tmp] = start;
            }
        }
    }
    return start;
}



int main(){
    Node p;
    Node q;
    cout << q.num << p.num << endl << p.id << q.id;
    return 0;
}
