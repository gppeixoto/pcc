#include "pmt_backend.h"

int main(int argc, char** argv){
	int c;
	bool has_edit_option = false;
	bool has_pattern_file_option = false;
	int count_option_num = 0;//count number of arguments

	int max_error = 0;//maximum edit distance allowed
	string pat_file;
	vector<string> patterns;
	vector<string> textfiles;


  	while (1){
        static struct option long_options[] =
		{
          {"help",     no_argument,       0, 'h'},
          {"edit",     required_argument, 0, 'e'},
          {"pattern",  required_argument, 0, 'p'},
          {0, 0, 0, 0}
        };
        int option_index = 0;

        c = getopt_long (argc, argv, "he:p:",long_options, &option_index);

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
    	ifstream infile(pat_file);

		if (!infile.good()){
			cout << "Arquivo de padrão << " << pat_file << " >> não existe" << endl;
		}

		string pat;
		while (infile >> pat){
			patterns.push_back(pat);
		}
    }

    if(argc -1 <= count_option_num){
    	cout << "Insira 1 ou mais arquivos de texto" << endl;
    }

    for (int i = count_option_num+1; i < argc; i++){
		string str = argv[i];
		//vector<string> matches = handle_wildcard(str);
		//for(string &match : matches){
			textfiles.push_back(str);
		//}
	}

	if(!has_edit_option || max_error == 0){//exact search
		if(patterns.size() == 1){//kmp
			string pat = patterns[0];
			for(string &txt : textfiles){
				run_kmp(txt, pat);
			}
		}else if(patterns.size() > 1){//aho

		}else{
			//error
		}
	}else{//approximate search

	}

	// Kmp obj = Kmp("abracadabra", "abra");

	// vector<int> occurrences = obj.run();
	// obj.print_vec(occurrences);
	
	return 0;
}
