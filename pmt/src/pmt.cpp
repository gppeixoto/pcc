#include "pmt_backend.h"


inline vector<string> getTextFiles(const string& pat){
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}

int main(int argc, char** argv){
	int c;
	bool has_edit_option = false;
	bool has_pattern_file_option = false;
    bool silent = false;
	int count_option_num = 0;//count number of arguments

	int max_error = 0;//maximum edit distance allowed
	string pat_file;
	vector<string> patterns;
	vector<string> textfiles;


  	while (1){
        static struct option long_options[] =
		{
            {"silent",   no_argument,       0, 's'},
            {"help",     no_argument,       0, 'h'},
            {"edit",     required_argument, 0, 'e'},
            {"pattern",  required_argument, 0, 'p'},
            {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long (argc, argv, "she:p:",long_options, &option_index);

        if (c == -1)//end
        	break;

        switch (c){
        	case 'e':
        		has_edit_option = true;
        		max_error = atoi(optarg);
        		count_option_num += 2;
        		break;
        	case 'p':
        		has_pattern_file_option = true;
        		pat_file = optarg;
        		count_option_num += 2;
        		break;
            case 's':
                silent = true;
                break;
        	case 'h':
        	case '?':
        	default:
        		//TODO add help function
        		break;
        }
    }
    if(!has_pattern_file_option){//read pattern from command line arg
    	count_option_num++;
    	string pat(argv[count_option_num]);
		patterns.push_back(pat);
    }else{//read patterns from file
        cout << "read from file" << endl;
    	ifstream infile(pat_file);

		if (!infile.good()){
			cout << "Arquivo de padrão << " << pat_file << " >> não existe" << endl;
		}

		string pat;
		while (infile >> pat){
			patterns.push_back(pat);
		}
    }

    if(argc - 1 <= count_option_num){
    	cout << "Insira 1 ou mais arquivos de texto" << endl;
    }

    for (int i = count_option_num+1; i < argc; i++){
		string str = argv[i];
		vector<string> files = getTextFiles(str);
		for(string &file : files){
			textfiles.push_back(file);
		}
	}

	if(!has_edit_option || max_error == 0){//exact search
		if(patterns.size() == 1){//kmp
			string pat = patterns[0];
			for(string &txt : textfiles){
				run_kmp(txt, pat, silent);
			}
		}else if(patterns.size() > 1){//aho
			for(string &txt : textfiles){
				run_aho(txt, patterns, silent);
			}
		}else{
			//error
		}
	}else if(has_edit_option){//approximate search
		for(string &txt : textfiles){
			for(string &pat : patterns){
                if(pat.length() < 64){
                    run_wu_manber(txt, pat, max_error, silent);
                }else{
				    run_sellers(txt, pat, max_error, silent);
                }
			}
		}
	}
	
	return 0;
}
