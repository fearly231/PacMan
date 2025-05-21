#  Pac-Man in C++ using SFML

Ten projekt to prosta implementacja klasycznego Pac-Mana stworzona w języku **C++** z użyciem biblioteki graficznej **SFML (Simple and Fast Multimedia Library)**.

---

##  Wymagania

- C++17 lub nowszy
- [SFML 2.5.1+](https://www.sfml-dev.org/download.php) (zainstalowane i skonfigurowane w projekcie)
- Kompilator zgodny z CMake lub Makefile (np. g++, MSVC)

---

##  Kompilacja

### Używając CMake (zalecane):

```bash
mkdir build
cd build
cmake ..
make
./PacMan
Zasady gry
Gracz porusza się Pac-Manem (YellowGuy) po planszy.
Celem jest zebranie wszystkich punktów (.) znajdujących się na mapie.
Czerwony duch (RedGhost) goni Pac-Mana.
Jeśli Pac-Man zbierze specjalny punkt (np. wisienkę), duch staje się "przestraszony" i zmienia kolor na niebieski.
Jeśli duch dotknie Pac-Mana, a nie jest przestraszony – gra się kończy.

















