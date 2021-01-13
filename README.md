# search_engine
Search Engine in UFMG discipline 

# directory dataset store html page in format compression 7z
# download and extract 7z
7z x html.7z.001

# extract file chilkat-9.5.0-x86_64-linux-gcc.tar 
# set environment variable
export LD_LIBRARY_PATH=\<path-computer\>/chilkat-9.5.0-x86_64-linux-gcc/lib

# compile code c++
g++ -o exercicio1 example_spider.cpp -L../chilkat/lib -lchilkat-9esolv -lpthread

# execute code c++
./exercicio1 url_seed
