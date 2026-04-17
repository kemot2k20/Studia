import string

def uprosc_zdanie(tekst, dl_slowa, liczba_slow):
    words = tekst.split()
    dobre_slowa = []
    for word in words:
        if len(dobre_slowa) == liczba_slow:
            break
        
        word = word.strip(string.punctuation)
        if len(word) <= dl_slowa:
            dobre_slowa.append(word)
    
    proste_zdanie = ' '.join(dobre_slowa)
    proste_zdanie += '.'
    return proste_zdanie

with open("PanTadeusz.txt", "r", encoding="utf-8") as file:
    tekst = file.read()

print(uprosc_zdanie(tekst, 10, 15))