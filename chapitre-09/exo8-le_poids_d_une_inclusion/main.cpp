#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

// Fonction pour vérifier si un fichier est un .cpp
bool estCpp(const string& nom) {
    if (nom.length() >= 4) {
        return nom.substr(nom.length() - 4) == ".cpp";
    }
    return false;
}

// Fonction pour vérifier si un fichier est un .h
bool estH(const string& nom) {
    if (nom.length() >= 2) {
        return nom.substr(nom.length() - 2) == ".h";
    }
    return false;
}

int main() {
    // Optimisation des entrées-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    // Graphe des inclusions directes : si A inclut B, alors B est inclus par A.
    // Pour propager l'impact d'une modification de X vers ceux qui l'incluent,
    // on a besoin du graphe inverse (qui inclut qui).
    // Inclus_par[b] contient tous les fichiers qui incluent directement b.
    map<string, vector<string>> inclus_par;
    set<string> tous_les_fichiers;
    vector<string> ordre_fichiers;

    for (int i = 0; i < N; ++i) {
        string fichier;
        cin >> fichier;
        tous_les_fichiers.insert(fichier);
        ordre_fichiers.push_back(fichier);

        int k;
        cin >> k;
        for (int j = 0; j < k; ++j) {
            string inc;
            cin >> inc;
            // On enregistre que 'fichier' inclut 'inc' -> donc 'inc' est inclus par 'fichier'
            inclus_par[inc].push_back(fichier);
        }
    }

    int Q;
    if (!(cin >> Q)) return 0;

    vector<string> questions(Q);
    for (int i = 0; i < Q; ++i) {
        cin >> questions[i];
    }

    // Fonction pour calculer le nombre de .cpp à recompiler si 'depart' change
    auto calculer_impact = [&](const string& depart) {
        set<string> cpp_recompiles;
        queue<string> file_attente;
        set<string> visites;

        file_attente.push(depart);
        visites.insert(depart);

        // Si le fichier modifié lui-même est un .cpp, il se recompile
        if (estCpp(depart)) {
            cpp_recompiles.insert(depart);
        }

        while (!file_attente.empty()) {
            string courant = file_attente.front();
            file_attente.pop();

            // Parcourir tous les fichiers qui incluent 'courant' directement
            if (inclus_par.count(courant)) {
                for (const string& parent : inclus_par[courant]) {
                    if (estCpp(parent)) {
                        cpp_recompiles.insert(parent);
                    }
                    if (visites.find(parent) == visites.end()) {
                        visites.insert(parent);
                        file_attente.push(parent);
                    }
                }
            }
        }

        return (int)cpp_recompiles.size();
    };

    // Traitement des questions
    for (const string& q : questions) {
        int r = calculer_impact(q);
        cout << q << " " << r << "\n";
    }

    // Recherche du PLUS_LOURD parmi les .h de la liste
    string plus_lourd_nom = "AUCUN";
    int max_impact = -1;

    for (const string& f : ordre_fichiers) {
        if (estH(f)) {
            int impact = calculer_impact(f);
            if (impact > max_impact) {
                max_impact = impact;
                plus_lourd_nom = f;
            }
        }
    }

    if (max_impact == -1) {
        cout << "PLUS_LOURD AUCUN\n";
    } else {
        cout << "PLUS_LOURD " << plus_lourd_nom << " " << max_impact << "\n";
    }

    return 0;
}