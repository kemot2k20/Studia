from itertools import product

class BladFormuly(Exception):
    pass

class BrakWartosciZmiennej(BladFormuly):
    def __init__(self, nazwa):
        super().__init__(f"Brak wartości dla zmiennej '{nazwa}'.")

class NiepoprawnyTypArgumentu(BladFormuly):
    def __init__(self, nazwa_klasy, typ):
        super().__init__(f"{nazwa_klasy} wymaga obiektów klasy Formula, otrzymano {typ}.")

class Formula:
    def __add__(self, other):
        return Or(self, other)

    def __mul__(self, other):
        return And(self, other)

    def oblicz(self, zmienne):
        raise NotImplementedError

    def __str__(self):
        raise NotImplementedError

    def _zmienne(self):
        raise NotImplementedError

    def tautologia(self):
        zmienne = sorted(list(self._zmienne()))
        for wartosci in product([False, True], repeat=len(zmienne)):
            mapka = dict(zip(zmienne, wartosci))
            if not self.oblicz(mapka):
                return False
        return True

    def uproszcz(self):
        return self

class Stala(Formula):
    def __init__(self, wartosc):
        self.wartosc = bool(wartosc)

    def oblicz(self, zmienne):
        return self.wartosc

    def __str__(self):
        return str(self.wartosc)

    def _zmienne(self):
        return set()

    def uproszcz(self):
        return self

class Zmienna(Formula):
    def __init__(self, nazwa):
        self.nazwa = nazwa

    def oblicz(self, zmienne):
        try:
            return zmienne[self.nazwa]
        except KeyError:
            raise BrakWartosciZmiennej(self.nazwa)

    def __str__(self):
        return self.nazwa

    def _zmienne(self):
        return {self.nazwa}

    def uproszcz(self):
        return self

class Not(Formula):
    def __init__(self, podformula):
        if not isinstance(podformula, Formula):
            raise NiepoprawnyTypArgumentu("Not", type(podformula).__name__)
        self.podformula = podformula

    def oblicz(self, zmienne):
        return not self.podformula.oblicz(zmienne)

    def __str__(self):
        return f"¬({self.podformula})"

    def _zmienne(self):
        return self.podformula._zmienne()

    def uproszcz(self):
        pod = self.podformula.uproszcz()
        if isinstance(pod, Stala):
            return Stala(not pod.wartosc)
        return Not(pod)

class And(Formula):
    def __init__(self, lewa, prawa):
        if not isinstance(lewa, Formula) or not isinstance(prawa, Formula):
            raise NiepoprawnyTypArgumentu("And", f"{type(lewa).__name__}, {type(prawa).__name__}")
        self.lewa = lewa
        self.prawa = prawa

    def oblicz(self, zmienne):
        return self.lewa.oblicz(zmienne) and self.prawa.oblicz(zmienne)

    def __str__(self):
        return f"({self.lewa} ∧ {self.prawa})"

    def _zmienne(self):
        return self.lewa._zmienne() | self.prawa._zmienne()

    def uproszcz(self):
        lewa = self.lewa.uproszcz()
        prawa = self.prawa.uproszcz()

        if isinstance(lewa, Stala) and lewa.wartosc:
            return prawa
        if isinstance(prawa, Stala) and prawa.wartosc:
            return lewa
        
        if (isinstance(lewa, Stala) and not lewa.wartosc) or (isinstance(prawa, Stala) and not prawa.wartosc):
            return Stala(False)
        
        return And(lewa, prawa)

class Or(Formula):
    def __init__(self, lewa, prawa):
        if not isinstance(lewa, Formula) or not isinstance(prawa, Formula):
            raise NiepoprawnyTypArgumentu("Or", f"{type(lewa).__name__}, {type(prawa).__name__}")
        self.lewa = lewa
        self.prawa = prawa

    def oblicz(self, zmienne):
        return self.lewa.oblicz(zmienne) or self.prawa.oblicz(zmienne)

    def __str__(self):
        return f"({self.lewa} ∨ {self.prawa})"

    def _zmienne(self):
        return self.lewa._zmienne() | self.prawa._zmienne()

    def uproszcz(self):
        lewa = self.lewa.uproszcz()
        prawa = self.prawa.uproszcz()

        if isinstance(lewa, Stala) and not lewa.wartosc:
            return prawa
        if isinstance(prawa, Stala) and not prawa.wartosc:
            return lewa
        
        if (isinstance(lewa, Stala) and lewa.wartosc) or (isinstance(prawa, Stala) and prawa.wartosc):
            return Stala(True)
        
        return Or(lewa, prawa)

f = Or(Not(Zmienna("x")), And(Zmienna("y"), Stala(True)))

print("Formuła:", f)
print("Dla x=False, y=True →", f.oblicz({"x": False, "y": True}))
print("Tautologia? ->", f.tautologia())
print("\n")

uproszczona = f.uproszcz()
print("Uproszczona formuła:", uproszczona)
print("\n")

try:
    f.oblicz({"x": True})
except BladFormuly as e:
    print("Błąd:", e)

print("\n")

f1 = Zmienna("x") + Not(Zmienna("x"))
print("Formuła: ", f1)
print("Tautologia? ->", f1.tautologia())
