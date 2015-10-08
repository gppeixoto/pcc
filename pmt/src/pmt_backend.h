#ifndef _PMT_BACKEND
#define _PMT_BACKEND

#include <string>

#include <vector>
#include <queue>
#include <iostream>
#include <cstring>

#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>


#define PMT_EXACT 				1
#define PMT_EXACT_PATTERNFILE 	2
#define PMT_APPROXIMATE			4

#define PMT_OK						0
#define PMT_ERR_NO_SUCH_OPTION		1
#define PMT_ERROR_TOO_LONG			2
#define PMT_ERROR_READING_FILE		4

#define ALPHABET_SIZE 			256

using namespace std;

void run_kmp(string &txtfile, string &pattern);

#endif