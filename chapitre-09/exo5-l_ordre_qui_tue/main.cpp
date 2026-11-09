#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Structure pour stocker une dépendance : a a besoin de b
struct Dependance {
    string a;
    string b;
};

int main() {
    // Optimisation des entrées-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Représentation des blocs : chaque bloc contient la liste des objets déclarés dans l'ordre
    vector<vector<string>> piles_blocs;
    piles_blocs.push_back({}); // Bloc global de la fonction

    unordered_set<string> objets_liberes;
    unordered_map<string, bool> est_vivant; // true si déclaré et pas encore libéré
    vector<Dependance> dependances;

    // Fonction pour libérer un objet et vérifier les dépendances
    auto liberer_objet = [&](const string& obj, const string& action) -> bool {
        // Si l'objet n'a jamais été déclaré ou est déjà libéré, on ignore (sauf pour action explicite FERME où on vérifie si déjà libéré)
        if (objets_liberes.count(obj)) {
            return true; // Déjà libéré, rien à faire
        }

        // Vérification des dépendances dans l'ordre de lecture
        for (const auto& dep : dependances) {
            if (dep.a == obj) {
                // Si b a besoin d'être vivant mais qu'il est déjà libéré -> PLANTAGE
                if (objets_liberes.count(dep.b)) {
                    cout << "PLANTAGE " << dep.a << " " << dep.b << "\n";
                    return false;
                }
            }
        }

        // Libération effective
        objets_liberes.insert(obj);
        est_vivant[obj] = false;
        cout << action << " " << obj << "\n";
        return true;
    };

    string ligne;
    while (getline(cin, ligne)) {
        if (ligne.empty()) continue;
        stringstream ss(ligne);
        string cmd;
        ss >> cmd;

        if (cmd == "{") {
            piles_blocs.push_back({});
        } else if (cmd == "}") {
            if (!piles_blocs.empty()) {
                // Destruction des objets du bloc courant dans l'ordre INVERSE de leur déclaration
                auto& bloc_courant = piles_blocs.back();
                for (auto it = bloc_courant.rbegin(); it != bloc_courant.rend(); ++it) {
                    const string& obj = *it;
                    if (est_vivant[obj]) {
                        if (!liberer_objet(obj, "DETRUIT")) {
                            return 0; // Arrêt au premier plantage
                        }
                    }
                }
                piles_blocs.pop_back();
            }
        } else if (cmd == "DECL") {
            string x;
            ss >> x;
            piles_blocs.back().push_back(x);
            est_vivant[x] = true;
        } else if (cmd == "DEPEND") {
            string a, b;
            ss >> a >> b;
            dependances.push_back({a, b});
        } else if (cmd == "FERME") {
            string x;
            ss >> x;
            // Un FERME sur un objet déjà libéré ne fait rien
            if (est_vivant.count(x) && est_vivant[x]) {
                if (!liberer_objet(x, "FERME")) {
                    return 0; // Arrêt au premier plantage
                }
            }
        }
    }

    // Fin de l'entrée : fermeture implicite des blocs restants (du dernier au premier)
    while (!piles_blocs.empty()) {
        auto& bloc_courant = piles_blocs.back();
        for (auto it = bloc_courant.rbegin(); it != bloc_courant.rend(); ++it) {
            const string& obj = *it;
            if (est_vivant[obj]) {
                if (!liberer_objet(obj, "DETRUIT")) {
                    return 0; // Arrêt au premier plantage
                }
            }
        }
        piles_blocs.pop_back();
    }

    // Si personne n'est mort par plantage, terminer par PROPRE
    cout << "PROPRE\n";

    return 0;
}