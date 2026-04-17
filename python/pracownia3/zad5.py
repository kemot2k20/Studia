import itertools

def solver(s1, s2, s3, oper):
    litery = set(s1 + s2 + s3)
    if len(litery) > 10:
        raise ValueError("Zbyt wiele unikalnych liter (maksymalnie 10).")

    for perm in itertools.permutations('0123456789', len(litery)):
        mapa = dict(zip(litery, perm))

        if mapa[s1[0]] == '0' or mapa[s2[0]] == '0' or mapa[s3[0]] == '0':
            continue

        l1 = int(''.join(mapa[c] for c in s1))
        l2 = int(''.join(mapa[c] for c in s2))
        l3 = int(''.join(mapa[c] for c in s3))

        if oper == '+' and l1 + l2 == l3:
            yield mapa
        elif oper == '-' and l1 - l2 == l3:
            yield mapa
        elif oper == '*' and l1 * l2 == l3:
            yield mapa
        elif oper == '/' and l2 != 0 and l1 / l2 == l3:
            yield mapa


def show(s1, s2, s3, oper, mapa):
    def change(slowo):
        return ''.join(mapa[l] for l in slowo)
    szer = max(len(s1), len(s2), len(s3)) + 4
    print(f"{s1:<{szer}} {change(s1):>{szer}}")
    print(f"{oper + ' ' + s2:<{szer}} {change(s2):>{szer}}")
    print(f"{'-' * (szer)} -> {'-' * (szer)}")
    print(f"{s3:<{szer}} {change(s3):>{szer}}")
    print()


# s1 = "KIOTO"
# s2 = "OSAKA"
# s3 = "TOKIO"
# oper = '+'

print("Podaj pierwsze slowo")
s1 = input()
print("Podaj drugie slowo")
s2 = input()
print("Podaj slowo wynikowe")
s3 = input()
print("Podaj operator")
oper = input()

s1 = s1.upper()
s2 = s2.upper()
s3 = s3.upper()

for mapa in solver(s1, s2, s3, oper):
    show(s1, s2, s3, oper, mapa)
