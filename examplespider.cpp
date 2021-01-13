#include <iostream>
#include <bits/stdc++.h>
#include "/home/claudiovaliense/projetos/chilkat/include/CkSpider.h" // lib local chilkat
#include <stdio.h> // deafult c++
#include <fstream> // out write file
#include <thread> // thead and multi-threads
#include <chrono> // time
#include <set>

using namespace std;
using namespace std::chrono; // calculate the time
map<string, bool> domain_extern; // link and status visit
map<string, set<string> > domain_and_sub; // domain and sub-url visited
map<string, long > domain_avg; // domain and sub-url visited

void CrowlerDomain(const char* url_domain, int repeat_domain){
    /* Url domain and repeat crowler in domain */
    CkSpider spider; // library crownler
    set<string> set_url; // set url                
    priority_queue<string> queue_links;
    long avg=0; // calculate average per domain     
    
    spider.AddUnspidered(url_domain); //  Add the 1st URL
    spider.put_Utf8(true); // encode    
    string domain = spider.domain();    
    
    for (int index_repeat = 0; index_repeat < repeat_domain; index_repeat++) {                               
        auto start = high_resolution_clock::now(); // calculate time        
        for (int i=0; i  < spider.get_NumOutboundLinks(); i++){
            string temp_url = spider.getOutboundLink(i);
            if (count(temp_url.begin(), temp_url.end(), '/') <= 4) 
                domain_extern[temp_url] = false; // set new link domain extern and no visit                                                
        }

        string url;
        for (int i=0; i < spider.get_NumUnspidered(); i++){             
            url = spider.getUnspideredUrl(i);
            //cout << "url: " <<url << endl;
            if (count(url.begin(), url.end(), '/') > 4 || domain_extern[url] == true) // skip url level 2
                spider.SkipUnspidered(i); 
        }        
        if (spider.CrawlNext() == true) { // sucess crawler
            string url = spider.lastUrl();
            if (count(url.begin(), url.end(), '/') > 4) continue;
            set_url.insert(url); // url visited
            
            domain_extern[url] = true; // no repeat url
            cout <<  "crawler url: " << url << ", count: " << count(url.begin(), url.end(), '/') << endl;
            auto stop = high_resolution_clock::now();
            long duration = duration_cast<milliseconds>(stop - start).count();
            avg = avg + duration;  
            
            replace( url.begin(), url.end(), '/', '$');
            queue_links.push(url);

            ofstream write_html; // write file html 
            write_html.open("/home/claudiovaliense/projetos/html/" + url);    
            write_html << spider.lastHtml();
            write_html.close();                       
        }
        else {            
            if (spider.get_NumUnspidered() == 0)  // no url crawler
                std::cout << "No more URLs to spider" << "\r\n";            
            else 
                std::cout << spider.lastErrorText() << "\r\n";            
        }
        spider.SleepMs(50); // limit wait
    }
    domain_and_sub[url_domain] = set_url;
    domain_avg[url_domain] = avg/repeat_domain;    
}

void save_in_file(string url){
    ofstream write_domain; // write file html 
    write_domain.open("/home/claudiovaliense/projetos/html/domains.txt", ios_base::app); // append file
    
    write_domain << url << ',';
    for (auto sub : domain_and_sub[url]) 
        write_domain << sub << ",";
    write_domain << domain_avg[url] << "\n";
}

int main(int argc, char *argv[]){    
    //thread t1(ChilkatSample, "https://claudiovaliense.github.io/backupDistribuido/", 3); // pagina criada para testes
    //t1.join(); // pagina criada para testes
    string seed = argv[1];
    CrowlerDomain(seed.c_str(), 5); // test input bash
    
    // seed set for students
    /*CrowlerDomain("https://www.uol.com.br", 5); 
    save_in_file("https://www.uol.com.br");
    CrowlerDomain("https://disney.com.br", 5);
    save_in_file("https://disney.com.br");
    CrowlerDomain("https://www.itau.com.br", 5);
    save_in_file("https://www.itau.com.br");*/
    
    int index=0;
    for (auto x : domain_extern){ // url extern in HashMap
        if (x.second == false){   
            string url = x.first;
            CrowlerDomain(url.c_str(), 5);    
            save_in_file(url);            
            cout << "domain_extern: " << url << " " << x.second << endl; 
        }
    }
    cout << "Time: " << endl;
    for (auto x : domain_avg)  
        cout << x.first << "," << x.second << endl; 
    return 0;
}

// backup
/*

void task1(string msg)
{
    cout << "task1 says: " << msg << endl;
}


spider.Initialize(url);
//cout << "number11: " << spider.get_NumUnspidered() << "spider.getUnspideredUrl(0): " 
//<< spider.getUnspideredUrl(0) << endl;
cout <<  spider.lastHtml();

//char x [] = url;
//const char url[100] = spider.lastUrl();
//replace(url.begin(), url.end(), '/', '$');
//cout << queue_links.top();

//
//char x [] = url;
//const char url[100] = spider.lastUrl();
//replace(url.begin(), url.end(), '/', '$');
//cout << queue_links.top();

//const bool existe = set_url.find( spider.getOutboundLink(0) ) != set_url.end();
//cout << "existe: " << set_url.count( spider.getOutboundLink(0) ) << endl;

//for (set<string>::iterator itr=set_url.begin(); itr!=set_url.end(); ++itr) 
//    cout << ' ' << *itr;


//while (number_bar > 3){
//    spider.SkipUnspidered(0); // skip url nivel 2
//}


//cout << "url nao visitada: " << spider.getUnspideredUrl(0) << endl;
//cout << "agente: " << spider.getUnspideredUrl(1) << endl;        
//return ;
//
//const char * url[1000];
//cout << "url visitas: " << spider.getSpideredUrl(0) << endl;
//cout << spider.lastHtmlDescription() <<endl;
// cout <<  spider.lastUrl() << "&" << spider.lastHtmlTitle(  ) << "\\\\" << endl;

//const char* seed = argv[1];
//const char* seed = "https://claudiovaliense.github.io/backupDistribuido/";
//ChilkatSample(argv[1], atoi( argv[2] ) ); // url
//ChilkatSample("https://claudiovaliense.github.io/backupDistribuido/", atoi( argv[2] ) ); // url

//for (auto x : domain_extern)  
//    cout << x.first << " " << x.second << endl; 

//for (set<string>::iterator itr=set_url.begin(); itr!=set_url.end(); ++itr)  
//    cout << "sub_url: " << *itr << endl;
const char * links[n+1];

    
    

std::atomic<bool> done(false); // Use an atomic flag.
std::thread t(ChilkatSample, "https://www.uol.com.br", 10  {
    std::this_thread::sleep_for(3s);
    done = true;
});
// Print status.
if (done) {
    std::cout << "Thread finished" << std::endl;
} else {
    std::cout << "Thread still running" << std::endl;
}


//std::this_thread::sleep_for(std::chrono::milliseconds(200));

//for (auto x : domain_and_sub)
//    for (set<string>::iterator itr=x.second.begin(); itr!=x.second.end(); ++itr)   
    //cout << "sub_url: " << *itr << endl;

//for (int i=0; i  < spider.get_NumSpidered(); i++) 
//cout << "url sim " << i <<": " << spider.getSpideredUrl(i) << endl;

//domain_and_sub.insert(spider.getOutboundLink(i));
t1.detach(); // executar e continuar execucao principal        

thread t1(CrowlerDomain, "https://www.uol.com.br", 3);   
thread t2(CrowlerDomain, "https://disney.com.br", 3);     
thread t3(CrowlerDomain, "https://www.itau.com.br", 3);    
t1.join();
t2.join();
t3.join();

for (int i=0; i < spider.get_NumSpidered(); i++) 
set_url.insert(spider.getSpideredUrl(i));

//thread t1(CrowlerDomain, x.first.c_str(), 3); 
//t1.join();

if (index %4 == 0){
    thread t1(CrowlerDomain, url.c_str(), 5); 
    t1.join();
}
if (index %4 == 1){
    thread t2(CrowlerDomain, url.c_str(), 5); 
    t2.join();
}
if (index %4 == 2){
    thread t3(CrowlerDomain, url.c_str(), 5); 
    t3.join();
}
if (index %4 == 3){
    thread t4(CrowlerDomain, url.c_str(), 5); 
    t4.join();
}
index = index+1;
//t1.join();
//t2.join();
//t3.join();
//t4.join();
*/
