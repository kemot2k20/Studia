# srodek okregu w (0, 0), dlugosc boku kwadratu = 2
# losuje x, y z zakresu (-1, 1)

import random
import math

def pi_aproximation(inner, count):
    return 4*inner / count

def shoot(count):
    inner = 0
    r = 1.0
    for _ in range(0, count): 
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        dist = x*x + y*y
        if dist <= r*r:
            inner += 1

    return inner

count = 1000000
result = pi_aproximation(shoot(count), count)
print(result, "\n")
print("error : ", abs(result - math.pi), "\n")