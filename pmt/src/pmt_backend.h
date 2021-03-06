#ifndef _PMT_BACKEND
#define _PMT_BACKEND

#include <string>

#include <vector>
#include <queue>
#include <iostream>
#include <cstring>
#include <set>

#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>

#include <glob.h>

#define FAIL 0
#define START 1
#define MIN_CHAR 32
#define MAX_CHAR 126
#define SIZE MAX_CHAR - MIN_CHAR
#define MAX_ST 30
#define CHECK_BIT(var,pos) ((var) & (1LL<<(pos)))

#define ALPHABET_SIZE 			256

using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;
typedef unsigned long long ull;

void run_kmp(string &txtfile, string &pattern, bool silent);
void run_sellers(string &txtfile, string &pattern, int maxError, bool silent);
void run_aho(string &txtfile, vs &patterns, bool silent);
void run_wu_manber(string &txtfile, string &pattern, int maxError, bool silent);

#endif