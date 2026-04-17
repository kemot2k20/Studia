import timeit

def isPrime(n):
    if n < 2: 
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    
    return True

def primes_imperative(n):
    res = []
    for i in range(n + 1):
        if isPrime(i):
            res.append(i)
    
    return res

def primes_composed(n):
    return [i for i in range(n + 1) if isPrime(i)]

def primes_functional(n):
    return list(filter(isPrime, range(n + 1)))

def time():
    rozmiary = [10, 20, 30, 40, 50, 100, 200, 500, 1000]
    print(f"{'n':<10}{'imperatywna':<20}{'składana':<20}{'funkcyjna':<20}")
    for n in rozmiary:
        t1 = timeit.timeit(lambda: primes_imperative(n), number = 10000)
        t2 = timeit.timeit(lambda: primes_composed(n), number = 10000)
        t3 = timeit.timeit(lambda: primes_functional(n), number = 10000)
        print(f"{n:<10}{t1:<20.6f}{t2:<20.6f}{t3:<20.6f}")

print(primes_imperative(50))
print(primes_composed(50))
print(primes_functional(50))
time()