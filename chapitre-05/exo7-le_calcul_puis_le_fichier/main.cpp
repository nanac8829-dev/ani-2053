#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long S;
    cin >> S;

    int N;
    cin >> N;

    long long totalMemoire = 0;
    int totalFlux = 0;
    int totalRefuses = 0;

    for (int i = 0; i < N; ++i) {
        string nom;
        long long frequence, canaux, bits, duree, fichier;

        cin >> nom >> frequence >> canaux
            >> bits >> duree >> fichier;

        // Seules les profondeurs autorisees sont acceptees.
        if (bits != 8 && bits != 16 &&
            bits != 24 && bits != 32) {
            cout << nom << " REFUSE\n";
            ++totalRefuses;
            continue;
        }

        // Effectuer toutes les multiplications avant la division.
        long long brut =
            frequence * canaux * (bits / 8) * duree / 1000;

        // Pourcentage entier, arrondi vers le bas.
        long long pourcent = fichier * 100 / brut;

        if (brut > S) {
            cout << nom << " " << brut << " "
                 << pourcent << " FLUX\n";
            ++totalFlux;
        } else {
            cout << nom << " " << brut << " "
                 << pourcent << " MEMOIRE\n";
            totalMemoire += brut;
        }
    }

    cout << "MEMOIRE " << totalMemoire << '\n';
    cout << "FLUX " << totalFlux << '\n';
    cout << "REFUSES " << totalRefuses << '\n';

    return 0;
}