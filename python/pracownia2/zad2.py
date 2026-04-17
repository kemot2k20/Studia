import sys

print(sys.getrecursionlimit())

def sudan(n, x, y):
    if (n == 0):
        return x + y

    if (y == 0):
        return x
    
    return sudan(n-1, sudan(n, x, y-1), sudan(n, x, y-1) + y)

print(sudan(2, 1, 2))

mem = {}
def sudan_faster(n, x, y):
    if (n == 0):
        return x + y

    if (y == 0):
        return x

    key = (n, x, y)
    if (key in mem):
        return mem[key]
    
    a = sudan(n, x, y-1)
    res = sudan(n-1, a, a + y)
    mem[key] = res
    return res

print(sudan_faster(2, 2, 2))