#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    // Optimisation des entrées-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int U;
    if (!(cin >> U)) return 0;

    vector<string> ordre_types;
    map<string, bool> necessite_definition; // true si INCLURE, false sinon
    set<string> vus;

    for (int i = 0; i < U; ++i) {
        string type, emploi;
        cin >> type >> emploi;

        if (vus.find(type) == vus.end()) {
            vus.insert(type);
            ordre_types.push_back(type);
            necessite_definition[type] = false;
        }

        // VALEUR, BASE, CORPS et TAILLE exigent la définition complète du type
        if (emploi == "VALEUR" || emploi == "BASE" || emploi == "CORPS" || emploi == "TAILLE") {
            necessite_definition[type] = true;
        }
    }

    int A;
    if (!(cin >> A)) A = 0;
    
    set<string> inclus_actuellement;
    for (int i = 0; i < A; ++i) {
        string t;
        cin >> t;
        inclus_actuellement.insert(t);
    }

    int nb_inclure = 0;
    int nb_anticiper = 0;
    int inclus_retirees = 0;

    // Affichage des décisions dans l'ordre de la première apparition
    for (const string& type : ordre_types) {
        bool inc = necessite_definition[type];
        if (inc) {
            cout << type << " INCLURE\n";
            nb_inclure++;
        } else {
            cout << type << " ANTICIPER\n";
            nb_anticiper++;
            // Si le type est actuellement inclus et qu'on peut l'anticiper, l'inclusion peut partir
            if (inclus_actuellement.find(type) != inclus_actuellement.end()) {
                inclus_retirees++;
            }
        }
    }

    // Affichage des totaux
    cout << "INCLURE " << nb_inclure << "\n";
    cout << "ANTICIPER " << nb_anticiper << "\n";
    cout << "INCLUSIONS_RETIREES " << inclus_retirees << "\n";

    return 0;
}