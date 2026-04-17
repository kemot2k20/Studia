
def erase_nonleter(text):
    new_text = ""
    for c in text:
        if c.isalpha():
            new_text += c

    return new_text

def check(text):
    l = 0
    r = len(text)-1
    while l < r:
        if text[l] != text[r]:
            return False
        l += 1
        r -= 1
    
    return True

def is_palindrom(text):
    if text == erase_nonleter(text):
        return check(text)
    
    return check(erase_nonleter(text).lower())

text = input()
print(is_palindrom(text))

# Eine güldne, gute Tugend: Lüge nie!
# Míč omočím.
# Kobyła ma mały bok.