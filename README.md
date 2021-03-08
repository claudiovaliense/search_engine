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

# TP 3, create inverted index -------------

# Structure inverted index
{ 'term' : [ [id_doc : [position_term, position2_term, ...]] , [id_doc2 : [position_term, position2,term, ...] ], ...  ], 
   'term2' : [....],
    ...
}

# Download module pre-requisite
python3.6 -m pip install html2text json

# Create files in plain text removing tag html in path dataset/html_to_path and create inverted file in dataset/inverted_index.json
python3.6 iv.py


# TP 4, create inverted file in disc and dictionary efficient search ------------------------

# Instalação do gerenciador de download kaggle
python3.6 -m pip install kaggle

# Adicionar username e chave para consegui realizar o download por autenticação
/.kaggle/kaggle.json

# Download do conjunto de dados
kaggle datasets download -d aldebbaran/html-br-collection

# Descompactar dados
unzip html-br-collection.zip

# Baixar e instalar parser com o objetivo de remover as tags htmls e deixar apenas o texto
#https://github.com/google/gumbo-parser

# Compilar solução proposta neste tp. obs: tem que ser sinal de acento
g++ -o tp4 tp4.cc \`pkg-config –cflags –libs gumbo\`

# Executar solução
./tp4
