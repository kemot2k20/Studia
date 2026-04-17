# Kompilator i flagi
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wshadow -fsanitize=address,undefined -O1

# Domyślny cel (jeśli nie podano pliku)
.DEFAULT_GOAL := help

# Kompilacja dowolnego pliku, np. make a -> kompiluje a.cpp
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Tryb debugowania: make debug a -> kompiluje a.cpp z -g i -DDEBUG
debug_%: %.cpp
	$(CXX) -std=c++17 -Wall -Wextra -Wshadow -fsanitize=address,undefined -g -DDEBUG -o $* $<

# Uruchamianie: make run a -> uruchamia ./a
run:
	@read -p "Podaj nazwę programu (np. a): " prog; ./$$prog

# Testowanie: make test a -> ./a < input.txt > output.txt
test:
	@read -p "Podaj nazwę programu (np. a): " prog; ./$$prog < input.txt > output.txt

# Czyszczenie plików binarnych
clean:
	rm -f a b c d e f g main *.o

# Pomoc
help:
	@echo "🧩 Makefile dla Codeforces"
	@echo "Użycie:"
	@echo "  make a          -> kompiluje a.cpp do ./a"
	@echo "  make debug_a    -> kompiluje a.cpp z flagami debugowania"
	@echo "  make run        -> uruchamia wybrany program"
	@echo "  make test       -> uruchamia wybrany program z input.txt > output.txt"
	@echo "  make clean      -> usuwa pliki wykonywalne"
