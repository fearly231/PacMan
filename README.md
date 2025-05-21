#  Pac-Man in C++ using SFML

Ten projekt to prosta implementacja klasycznego Pac-Mana stworzona w języku **C++** z użyciem biblioteki graficznej **SFML (Simple and Fast Multimedia Library)**.

---

##  Wymagania

- C++17 lub nowszy
- [SFML 3.0.0+](https://www.sfml-dev.org/download) (zainstalowane i skonfigurowane w projekcie)
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
```
## Zasady gry
- Gracz porusza się Pac-Manem po planszy.
- Celem jest zebranie wszystkich punktów (.) znajdujących się na mapie.
- Duchy gonią Pac-Mana.
- Jeśli Pac-Man zbierze specjalny punkt, duchy stają się "przestraszone" i zmieniają kolor na niebieski.
- Jeśli duch dotknie Pac-Mana, a nie jest przestraszony – gra się kończy.

## 🎮 Sterowanie

- `⬅`, `⬆`, `⬇`, `⮕` – poruszanie się Pac-Mana
- Zbieraj wszystkie punkty, by wygrać!
- Uważaj na ducha – jeśli nie jest przerażony, przegrywasz przy kontakcie.

 
















