import html2text
import os
import json  # Manipulate extension json

def html_to_text():
    """Html to text"""
    h = html2text.HTML2Text()
    h.ignore_links = True; h.ignore_images = True
    for _, _, files in os.walk('dataset/html'):
        for file in files:
            vet = []
            try:
                for line in open(f'dataset/html/{file}'):
                    vet.append(line)
                vet = " ".join(vet)
                writer = open(f'dataset/html_to_text/{file}', 'w')
                writer.write( h.handle(vet) )
                writer.close()
            except: # erro na abertura do arquivo
                continue 

def inverted_index():
    """Create inverted index"""
    iv = dict()
    for _, _, files in os.walk('dataset/html_to_text'):
        for file in files:
            vet = []
            try:
                for line in open(f'dataset/html_to_text/{file}'):
                    vet.append(line.strip())
                vet = " ".join(vet)
                vet = vet.split(" ")
                terms_doc = dict()
                for index_term in range(len(vet)):
                    if terms_doc.get(vet[index_term]) == None:
                        terms_doc[vet[index_term]] = []
                    terms_doc[vet[index_term]].append(index_term)
                for t_local, posi in terms_doc.items(): # termos e posicoes
                    if iv.get(t_local) == None:
                        iv[t_local] = []
                    # adiciona no indice invertido o termo e suas respectivas posicoes
                    iv[t_local].append( (file, posi) )
            except: # skip in documents with erro lecture
                print('Erro leitura')
                continue
    json.dump(iv, open("dataset/inverted_index.json", "w") ) # salva o indice invertido em um .json

def statistics_iv(iv):
    """Calcula estatistica sobre o indice"""
    terms_unicos = len(iv.keys())
    print(f"Terms distincts: {terms_unicos}")
    avg_position = 0
    for positions in iv.values():
        avg_position += len(positions)
    print(f"Avg positions terms: {avg_position / terms_unicos}")
    

html_to_text()
inverted_index()
#statistics_iv( json.load( open("dataset/inverted_index.json") ) )
