# Crossy Road projekt - Projekt R

## Pokretanje projekta:

Koristi se [CMake](https://cmake.org/) sustav.

Prvo je potrebno napraviti `build` folder, može se zvati bilo kako međutim Git je postavljen da ignorira `build/`.
Zatim se iz tog direktorija mogu pokrenuti CMake naredbe.

```
mkdir build
cd build
cmake ..
cmake --build .
```

Eventualno za veću brzinu build-a se može koristiti `cmake --build . -j` umjesto `cmake --build .`, čime se koristi više jezgri računala.

Ako sve prođe kako spada, izvršiva datoteka `Projekt-R` će se nalaziti u direktoriju `build`.
