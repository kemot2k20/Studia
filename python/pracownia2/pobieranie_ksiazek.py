# ksiazki nowo pobrane zawieraja niekiedy dlugi generowany tekst w jezyku angielskim.
# Pozostawienie tego tekstu wplywa na wynik predykcji
# Nie pobierac ponownie jesli posiada sie juz ksiazki bez wygenerowanego tekstu

import os, requests

def download_gutenberg_book(book_id, folder):
    url = f"https://www.gutenberg.org/files/{book_id}/{book_id}-0.txt"
    os.makedirs(folder, exist_ok=True)
    path = os.path.join(folder, f"{book_id}.txt")

    r = requests.get(url)
    if r.ok:
        open(path, "w", encoding="utf-8").write(r.text)
        print(f"✅ Zapisano: {path}")
    else:
        print(f"❌ Błąd {r.status_code} przy pobieraniu {book_id}")

folder = "/mnt/c/Users/tomas/Desktop/studia/sem5/python/pracownia2/ksiazki" # Dodac sciezke jesli potrzeba 

# 3 angielskie, 3 hiszpańskie, 3 polskie, 3 testowe
lista_ksizek = [1513, 11, 1661, 58221, 69552, 73257, 31536, 28121, 33598, 17473, 2542, 2000]
for ksiazka in lista_ksizek:
    download_gutenberg_book(ksiazka, folder)