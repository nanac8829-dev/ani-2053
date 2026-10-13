
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long D, C;
    cin >> D >> C;

    int N;
    cin >> N;

    vector<long long> t(N);

    for (int i = 0; i < N; ++i) {
        cin >> t[i];
    }

    long long voixMax = 0;
    long long retardMax = 0;
    long long coupes = 0;

    long long finChargement = 0;

    for (int i = 0; i < N; ++i) {
        // 1. Compter les voix encore actives a l'instant t[i].
        long long voix = 0;

        for (int j = 0; j <= i; ++j) {
            if (t[j] + D > t[i]) {
                ++voix;
            }
        }

        // 2. Calculer le debut et la fin du chargement.
        long long debutChargement = max(t[i], finChargement);
        finChargement = debutChargement + C;

        long long retard = finChargement - t[i];

        // 3. Une seule voix : la lecture est coupee
        // si la suivante commence strictement avant sa fin.
        bool coupe = false;

        if (i + 1 < N && t[i + 1] < t[i] + D) {
            coupe = true;
        }

        cout << t[i] << " "
             << voix << " "
             << retard << " "
             << (coupe ? "COUPE" : "ENTIER") << '\n';

        voixMax = max(voixMax, voix);
        retardMax = max(retardMax, retard);

        if (coupe) {
            ++coupes;
        }
    }

    cout << "VOIX_MAX " << voixMax << '\n';
    cout << "RETARD_MAX " << retardMax << '\n';
    cout << "COUPES " << coupes << '\n';

    return 0;
}