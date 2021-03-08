/* Name: Claudio Moises Valiense de Andrade, Licence: MIT */

#include <fstream> // read file
#include <iostream> // read file
#include <stdlib.h> // function main in c++
#include <string> // string in c++
#include "gumbo.h" // clear text
#include "json.hpp" // work json
#include <bits/stdc++.h> // split string
#include <math.h> //  log10 
using namespace std; // function c++
using json = nlohmann::json; // json in c++
#include <chrono> // time
using namespace std::chrono; // calculate the time

static std::string cleantext(GumboNode* node) {
    /*Remove tags html.*/
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT &&  node->v.element.tag != GUMBO_TAG_SCRIPT &&  node->v.element.tag != GUMBO_TAG_STYLE) {
        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = cleantext((GumboNode*) children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append(" ");
            }
            contents.append(text);
        }
        return contents;
    } else {
        return "";
    }
}

void divide_in_files(){
    /*Clear html and divide in files.*/
    ifstream input("../../kaggle/collection.jl");
    int id_doc=0;
    ofstream map_id;
    map_id.open("map_id.txt");
    for( std::string line; getline( input, line ); ){
        ofstream saida;
        saida.open("dataset_tp/" +to_string(id_doc) +".txt");
        auto line_json = json::parse(line);
        string url = line_json.value("url", "oops");
        string contents = line_json.value("html_content", "oops");
        GumboOutput* output = gumbo_parse(contents.c_str());
        saida << cleantext(output->root);
        map_id << id_doc << "," << url << endl;
        id_doc++;
    }
}

vector<string> split(const string& str, const string& delim){
    /*split string in n terms*/
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do{
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

vector<string> split2(const string& str, const string& delim){
    /*split string in two parts*/
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    pos = str.find(delim, prev);
    if (pos == string::npos) pos = str.length();
    string token = str.substr(prev, pos-prev);
    if (!token.empty()) tokens.push_back(token);
    token = str.substr(pos+1, str.length() );
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

string lower_string(string str){
    /*Convert string in lowercase; ("HeLlo"); "hello"*/
    for(auto& c : str){
        c = tolower(c);
    }
    return str;
}

void file_temp(){
    /*Create file temporary that store terms, doc and position.*/
    int id_doc = 0;
    map<string,int> vocab;
    ofstream saida, map_word;
    map_word.open("map_word.txt"); 
    saida.open("file_temp.txt");
    int index_token=0; 
    int total_docs = 1000068;
    
    for (int id_doc=0; id_doc< total_docs; id_doc++){
        ifstream input("dataset_tp/" +to_string(id_doc) +".txt");
        for(string line; getline( input, line ); ){
            vector<string> tokens = split(line, " ");
            for ( int i=0; i < tokens.size(); i++){
                if ( vocab.find(tokens[i]) == vocab.end() ){
                    vocab[tokens[i]] = index_token;
                    map_word << index_token << ',' << tokens[i] << endl;
                    index_token+=1;
                }
                saida << vocab[tokens[i]] << ',' << id_doc << ',' << i << endl; // token, doc, position
            }
        }
    }
}

void execute_runs(){
    /*Particiona para execucao da grande tabela temporaria.*/
    ifstream input("file_temp.txt");
    for(string line; getline( input, line ); ){
        ofstream saida;
        vector<string> idtoken_iddoc_pos  = split(line, ",");
        //cout << idtoken_iddoc_pos[0] << endl;
        saida.open("terms/" +idtoken_iddoc_pos[0] +".txt", std::ios::app ); // add end file
        saida << line << endl;
    }
}

void merge_terms(){
    /* Merge terms in single file (create inverted index)
     * Cria dicionario com os termos, posicao do inicio, fim e idf no dicionario do arquivo invertido.
     * */
    ofstream saida("inverted_index.txt");
    ifstream input("map_word.txt"); int count_words=0;
    map<int,string> vocab;
    for(string line; getline( input, line ); ){
        vector<string> id_term  = split2(line, ",");
        vocab[stoi(id_term[0])] = id_term[1];
        count_words++;
        //cout  << count_words << endl;
        //cout << id_term[1] << endl;
    }
    int cont_doc_term = 0; ofstream saida_dicionario("dicionario_inverted_index.txt");
    int ini=0; int fim=0; int total_docs = 1000068;
    for (int index_word=0; index_word < count_words; index_word++){
        ini = fim;
        ifstream input("terms/" +to_string(index_word) +".txt");
        int cont_docs_term=0; // cont docs with term
        string last_doc = ""; // store id last doc
        for(string line; getline( input, line ); ){
            vector<string> idterm_iddoc_pos  = split(line, ",");
            if (idterm_iddoc_pos[1] != last_doc){
                cont_docs_term++;
            }
            last_doc = idterm_iddoc_pos[1];
            saida << line << endl;
            cont_doc_term++;
        }
        cout << index_word << endl;
        if (index_word == 23){
            cout << cont_docs_term << endl; return; }
        fim = cont_doc_term;
        saida_dicionario << vocab[index_word] << ',' <<  ini << ',' << fim << ',' << log10( (double) total_docs / cont_docs_term) << endl;
    }
}

vector< pair<string, double> > top_k(map<string, double> mymap, int k){
    /*Top k in hashmap*/
    vector< pair<string, double> > top_k_map(k);
    partial_sort_copy(mymap.begin(), mymap.end(), top_k_map.begin(), top_k_map.end(),
                       [](pair<const string, double> const& l, pair<const string, double> const& r)
                       {
                           return l.second > r.second;
                       });
    return top_k_map;
}

void model_vector(string q){
    vector<string> terms_q = split(q, " ");
    ifstream input("map_word.txt"); int count_words=0;
    map<string,string> vocab;
    int total_docs = 1000068;
    for(string line; getline( input, line ); ){ // identificador do termo
        vector<string> id_term  = split2(line, ",");
        vocab[id_term[1]] = id_term[0];
    } 
    map<string, double> term_tfidf_docs; // doc and tfidf
    for (auto term : terms_q){ // identifica os documentos que os termos ocorre
        map<string, int> doc_freq; // identificador do documento e a frequencia do termo
        ifstream input("terms/" +vocab[ term ] +".txt");
        int freq_doc = 1;
        int cont_docs = 0;
        string last_doc = ""; // store id last doc
        
        for(string line; getline( input, line ); ){ // todos os documentos que o termo aparece
            vector<string> idterm_iddoc_pos  = split(line, ",");
            if (idterm_iddoc_pos[1] != last_doc){ // other document
                last_doc = idterm_iddoc_pos[1];
                doc_freq[last_doc] = freq_doc;
                freq_doc=1;
                cont_docs++;
            }else{
                freq_doc++;
            }
        }
        double idf_term = log10( (double) total_docs / cont_docs);
        for (auto doc_freq_local : doc_freq){ // calcula o tfidf dos documentos que aparecem o termo
            double tf_idf = 1 + log10(doc_freq_local.second) * idf_term;
            if (term_tfidf_docs.count( doc_freq_local.first )){ // sum score de diferentes termos
                term_tfidf_docs[doc_freq_local.first] = term_tfidf_docs[doc_freq_local.first] + tf_idf;
            }else{
                term_tfidf_docs[doc_freq_local.first] = tf_idf;
            }
        }
    }
    ifstream input_url("map_id.txt"); // map the url
    map<string,string> map_id_url;
    for(string line; getline( input_url, line ); ){ // identificador do termo
        vector<string> id_url  = split2(line, ",");
        map_id_url[id_url[0]] = id_url[1];
    } 
    vector< pair< string, double>> top_five = top_k(term_tfidf_docs, 5);
    for (auto x : top_five) 
        cout << map_id_url[ x.first ] << endl; // << ", " << x.second << endl;
}

int main(int argc, char** argv) {
    auto start = high_resolution_clock::now();
    /*Function main in c++*/
    //divide_in_files();
    //file_temp();
    //execute_runs();
    //merge_terms();
    cout << "Query: " << argv[1] << endl;
    model_vector(argv[1]);
    auto stop = high_resolution_clock::now();
    long duration = duration_cast<milliseconds>(stop - start).count();
    cout << "Time(milliseconds): " << duration << endl;
}

/*
 // backup


string RemoveSpecials(string str)
{
	int i=0,len=str.length();
	while(i<len)
	{
		char c=str[i];
		if(((c>='0')&&(c<='9'))||((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))) 
		{
			if((c>='A')&&(c<='Z')) str[i]+=32; //Assuming dictionary contains small letters only.
			++i;
		}
		else
		{
			str.erase(i,1);
			--len;
		}
	}
	return str;
}
  
const char b[] = "ábaã";
cout << b[1] << endl;
cout << x.size() << endl;
wchar_t a = (L"%c", x[1]);
cout << a;
//wchar_t c  = Lx[0];
//cout << c;
//cout << replace_accent(x);
//char name[20] = "Cláudio";
//cout << removeAccented(name);

char* removeAccented( char* str ) {
    char *p = str;
    while ( (*p)!=0 ) {
        const char*
        //   "ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"
        tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
        unsigned char ch = (*p);
        if ( ch >=192 ) {
            (*p) = tr[ ch-192 ];
        }
        ++p; // http://stackoverflow.com/questions/14094621/
    }
    return str;
}

string replace_accent(string str){
    int x = 0;
    for( int i=0; i < str.size(); i++){
            std::cout << std::hex << (int)str[i] << endl;
        /cout << int(str[i]) << endl;
        if (str[i] == 133){ 
            str[i] ='p';
        }
    }
    return str;
}
*/
