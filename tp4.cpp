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
    /*split string in terms*/
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
    //for (auto x : vocab)  
    //    cout << x.first << "," << x.second << endl; 
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
        vector<string> id_term  = split(line, ",");
        vocab[stoi(id_term[0])] = id_term[1];
        count_words++;
    }
    int cont_doc_term = 0; ofstream saida_dicionario("dicionario_inverted_index.txt");
    int ini=0; int fim=0; int total_docs = 1000068;
    for (int index_word=0; index_word < count_words; index_word++){
        ini = fim;
        ifstream input("terms/" +to_string(index_word) +".txt");
        for(string line; getline( input, line ); ){
            saida << line << endl;
            cont_doc_term++;
        }
        fim = cont_doc_term;
        saida_dicionario << vocab[index_word] << ',' <<  ini << ',' << fim << ',' << log10( (double) total_docs / cont_doc_term) << endl;
    }
}

int main(int argc, char** argv) {
    /*Function main in c++*/
    //divide_in_files();
    //file_temp();
    //execute_runs();
    merge_terms();
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
