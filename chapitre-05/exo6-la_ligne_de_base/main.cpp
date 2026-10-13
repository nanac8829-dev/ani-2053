#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>

using namespace std;

struct Glyphe {
    long long avance;
    long long x0, y0, x1, y1;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int G;
    cin >> G;

    unordered_map<char, Glyphe> glyphes;

    for (int i = 0; i < G; ++i) {
        char c;
        Glyphe g;

        cin >> c >> g.avance
            >> g.x0 >> g.y0 >> g.x1 >> g.y1;

        glyphes[c] = g;
    }

    int K;
    cin >> K;

    unordered_map<string, long long> crenages;

    for (int i = 0; i < K; ++i) {
        string couple;
        long long k;

        cin >> couple >> k;
        crenages[couple] = k;
    }

    string texte;
    long long ox, oy;

    cin >> texte >> ox >> oy;

    long long x = ox;

    long long minx = numeric_limits<long long>::max();
    long long miny = numeric_limits<long long>::max();
    long long maxx = numeric_limits<long long>::lowest();
    long long maxy = numeric_limits<long long>::lowest();

    int absents = 0;
    bool dessine = false;

    for (size_t i = 0; i < texte.size(); ++i) {
        char c = texte[i];

        auto it = glyphes.find(c);

        // Un caractere absent ne deplace pas le curseur
        // et ne declenche aucun crenage.
        if (it == glyphes.end()) {
            cout << c << " ABSENT\n";
            ++absents;
            continue;
        }

        const Glyphe& g = it->second;

        // Afficher la position du curseur du caractere.
        cout << c << " " << x << '\n';

        // Dessiner uniquement si le rectangle a une aire positive.
        if (g.x1 > g.x0 && g.y1 > g.y0) {
            long long gauche = x + g.x0;
            long long haut = oy + g.y0;
            long long droite = x + g.x1;
            long long bas = oy + g.y1;

            minx = min(minx, gauche);
            miny = min(miny, haut);
            maxx = max(maxx, droite);
            maxy = max(maxy, bas);

            dessine = true;
        }

        // Avancer le curseur.
        x += g.avance;

        // Appliquer le crenage avec le caractere suivant.
        if (i + 1 < texte.size()) {
            string couple;
            couple += c;
            couple += texte[i + 1];

            auto itCrenage = crenages.find(couple);

            if (itCrenage != crenages.end()) {
                x += itCrenage->second;
            }
        }
    }

    cout << "CURSEUR " << x << '\n';

    if (!dessine) {
        cout << "BOITE AUCUNE\n";
        cout << "MONTE 0\n";
        cout << "DESCEND 0\n";
        cout << "ECRAN RIEN\n";
    } else {
        cout << "BOITE "
             << minx << " "
             << miny << " "
             << maxx << " "
             << maxy << '\n';

        long long monte = (miny < oy) ? oy - miny : 0;
        long long descend = (maxy > oy) ? maxy - oy : 0;

        cout << "MONTE " << monte << '\n';
        cout << "DESCEND " << descend << '\n';

        if (maxy <= 0) {
            cout << "ECRAN HORS\n";
        } else if (miny < 0) {
            cout << "ECRAN COUPE\n";
        } else {
            cout << "ECRAN VISIBLE\n";
        }
    }

    cout << "ABSENTS " << absents << '\n';

    return 0;
}