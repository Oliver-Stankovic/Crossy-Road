# osnovna neuronska mreža

Jednostavan primjer neuronske mreže. Zasad samo računa izlaze, nema učenja i nije povezana sa igricom ni genetskim algoritmom.

## datoteke

- `NeuralNetwork.h`: neuron i mreža, sa sigmoidom, prolazom unaprijed i parametrima
- `main.cpp`: mali primjer korištenja mreže 2 → 2 → 1
- `CMakeLists.txt`: upute za izgradnju programa
- `.gitignore`: preskače automatski napravljenu mapu build

## što model radi

Svaki neuron množi ulaze težinama, dodaje bias i primjenjuje sigmoidu.
Izlazi jednog sloja postaju ulazi sljedećeg. Ulazni sloj samo predaje podatke.
Svi računski slojevi, uključujući izlazni, koriste sigmoidu.

Broj slojeva i neurona odabire se pri stvaranju mreže:

Funkcije:

- `forward(inputs)`: izračuna i vrati listu izlaza
- `parameterCount()`: vrati ukupan broj težina i biasa
- `getParameters()`: vrati sve parametre kao jednu listu
- `setParameters(parameters)`: postavi sve parametre iz liste

## očekivani rezultat primjera

mreza: 2 -> 2 -> 1
broj parametara: 9

ulaz: [0.000000, 0.000000] -> izlaz: 0.500000
ulaz: [1.000000, 0.000000] -> izlaz: 0.557509
ulaz: [0.000000, 1.000000] -> izlaz: 0.557509
ulaz: [1.000000, 1.000000] -> izlaz: 0.613516

## pokretanje
cmake -S . -B build
cmake --build build
./build/OsnovnaMreza