import numpy as np
from numpy.linalg import norm
import os

def cosine_similarity(A, B):
    return np.dot(A, B) / (norm(A) * norm(B))

train_data =   [('1513', 'ENG'), ('11', 'ENG'), ('1661', 'ENG'), 
            ('58221', 'ESP'), ('69552', 'ESP'), ('73257', 'ESP'), 
            ('31536', 'POL'), ('28121', 'POL'), ('33598', 'POL')]

languages = ['ENG', 'ESP', 'POL']

char_list = list("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZąćęłńóśżźĄĆĘŁŃÓŚŻŹáéíóúüñÁÉÍÓÚÜÑ ,.!?;:")
char_to_index = {c:i for i, c in enumerate(char_list)}
vec = {}
for lang in languages:
    vec[lang] = [0] * len(char_list)

folder = "/mnt/c/Users/tomas/Desktop/studia/sem5/python/pracownia2/ksiazki"

def count(books, lang):
    c = 0
    for book in books:
        if book[1] == lang:
            c += 1

    return c


# preprocessing
def process_train_data(train_data):
    for book in train_data:
        lang = book[1]
        filename = book[0]
        filepath = os.path.join(folder, f"{filename}.txt")
        v = [0] * len(char_list)
        sum = 0
        with open(filepath, "r", encoding="utf-8") as f:
            for line in f:
                line = line.lower()
                for c in line:
                    if c in char_list:
                        v[char_to_index[c]] += 1
                        sum += 1

        v = [x / sum for x in v]
        vec[lang] = np.array(vec[lang]) + np.array(v)

    Count = {}
    for lang in languages:
        Count[lang] = count(train_data, lang)
    for lang in languages:
        vec[lang] = [x / Count[lang] for x in vec[lang]]
        

process_train_data(train_data)

def validation(test_data):
    result = {}
    for book, _ in test_data.items():
        filepath = os.path.join(folder, f"{book}.txt")
        v = [0] * len(char_list)
        sum = 0
        with open(filepath, "r", encoding="utf-8") as f:
            for line in f:
                line = line.lower()
                for c in line:
                    if c in char_list:
                        v[char_to_index[c]] += 1
                        sum += 1

        v = [x / sum for x in v]
        result[book] = v
    
    return result

    
test_data = {'17473': 'POL', '2542': 'ENG', '2000': 'ESP', 'manual_pol': 'POL', 'manual_esp': 'ESP', 'manual_eng': 'ENG', 'test': 'POL'}

result = validation(test_data)

similarity = {}
for book, v in result.items():
    similarity[book] = []
    for lang in languages:
        similarity[book].append((cosine_similarity(v, vec[lang]), lang))

    similarity[book].sort(reverse=True)

for book, sim in similarity.items():
    print(book, test_data[book], sim, "\n")


# softmax na koniec zeby uzyskac pp
def softmax(scores, T=0.1):
    scores = np.array(scores, dtype=float)
    exp_scores = np.exp(scores / T)
    return exp_scores / exp_scores.sum()


for book, sim in similarity.items():
    scores = [s for s, l in sim]
    langs = [l for s, l in sim]
    print("\n", book, test_data[book], "\n")
    probs = softmax(scores, T=0.1)
    for lang, prob in zip(langs, probs):
        print(f"{lang}: {prob:.4f}")


# bez usuniecia zapisow po angielsku generowanych przez Project Gutenberg jest gorsza dokladnosc (dla przykladu 17473 Angielski mial najwieksze pp)