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

#define ALPHABET_SIZE 			256

using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;

void run_kmp(string &txtfile, string &pattern);
void run_sellers(string &txtfile, string &pattern, int maxError);
void run_aho(string &txtfile, vs &patterns);

#endif