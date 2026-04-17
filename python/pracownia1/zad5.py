
def common_prefix(arr):
    arr = [s.lower() for s in arr] # ciekawa skladnia
    prefix = ""
    flag = True
    while flag:
        l = len(prefix)
        mapa = {}
        for s in arr:
            if len(s) <= l:
                continue

            pref = s[:l+1]
            mapa[pref] = mapa.get(pref, 0) + 1
        
        p = ""
        maks = 0
        for key, value in mapa.items():
            if value > maks:
                maks = value
                p = key

        if maks < 3:
            flag = False
        else:
            prefix = p

    return prefix

arr = ["Cyprian", "cyberotoman", "cynik", "ceniąc", "czule"]
arr2 = ["aaabab", "aabba", "ababa", "aabbaba", "ab", "bbba", "aabbbaba"]
print(common_prefix(arr), '\n', common_prefix(arr2))