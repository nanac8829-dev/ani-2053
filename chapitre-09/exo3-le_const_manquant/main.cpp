#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct Methode {
    string nom;
    string declaree; // "const" ou "-"
    int modifie;
    vector<string> appels;
};

int main() {
    // Optimisation des entres-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<Methode> methodes(n);
    map<string, int> nomVersIdx;

    for (int i = 0; i < n; ++i) {
        cin >> methodes[i].nom >> methodes[i].declaree >> methodes[i].modifie;
        int k;
        cin >> k;
        methodes[i].appels.resize(k);
        for (int j = 0; j < k; ++j) {
            cin >> methodes[i].appels[j];
        }
        nomVersIdx[methodes[i].nom] = i;
    }

    // Étape 1 : Déterminer quelles méthodes pourraient être const
    // Une méthode peut l'être si elle n'est pas déjà déclarée const, si elle ne modifie rien,
    // et si toutes ses appelées sont soit déjà déclarées const, soit pourraient l'être.
    // Comme les appels ne forment pas de cycle, on peut propager ou résoudre par points fixes.
    
    // On va stocker l'état "pourrait être const" (hors déjà const)
    vector<bool> pourrait(n, false);
    
    // Fonction récursive avec mémoïsation pour vérifier si une méthode peut être const
    // (ou si elle l'est déjà)
    map<string, bool> cacheValidite;

    // Pour l'évaluation, une méthode est valide (peut être const ou l'est déjà) si :
    // - elle est déjà const, OU
    // - modifie == 0 et toutes ses appelées sont valides.
    
    // Étant donné qu'il n'y a pas de cycles, une simple vérification itérative par points fixes ou récursion fonctionne.
    // Faisons plusieurs passes de vérification de validité si nécessaire, ou résolvons par dépendance.
    
    // Construisons un graphe d'appel inverse ou évaluons par propagation :
    // Une méthode est "deja_ou_pourrait_const"
    vector<bool> estConstOuPourrait(n, false);
    for (int i = 0; i < n; ++i) {
        if (methodes[i].declaree == "const") {
            estConstOuPourrait[i] = true;
        }
    }

    bool changement = true;
    while (changement) {
        changement = false;
        for (int i = 0; i < n; ++i) {
            if (methodes[i].declaree == "const") continue;
            if (estConstOuPourrait[i]) continue;

            if (methodes[i].modifie == 1) {
                continue;
            }

            bool toutesAppeleesValides = true;
            for (const string& app : methodes[i].appels) {
                int idxApp = nomVersIdx[app];
                if (!estConstOuPourrait[idxApp]) {
                    toutesAppeleesValides = false;
                    break;
                }
            }

            if (toutesAppeleesValides) {
                estConstOuPourrait[i] = true;
                pourrait[i] = true;
                changement = true;
            }
        }
    }

    // Récupérer la liste POURRAIT (dans l'ordre de lecture)
    vector<string> listePourrait;
    for (int i = 0; i < n; ++i) {
        if (pourrait[i]) {
            listePourrait.push_back(methodes[i].nom);
        }
    }

    // Étape 2 : Trouver les RACINES
    // Une racine est une méthode de POURRAIT dont toutes les méthodes appelées sont DÉJÀ déclarées const.
    // (Une méthode qui n'appelle rien est donc naturellement une racine).
    vector<string> listeRacines;
    for (int i = 0; i < n; ++i) {
        if (pourrait[i]) {
            bool toutesDejaConst = true;
            for (const string& app : methodes[i].appels) {
                int idxApp = nomVersIdx[app];
                if (methodes[idxApp].declaree != "const") {
                    toutesDejaConst = false;
                    break;
                }
            }
            if (toutesDejaConst) {
                listeRacines.push_back(methodes[i].nom);
            }
        }
    }

    // Étape 3 : Calculer le nombre de PASSES
    // Une passe déclare const, toutes en même temps, les méthodes de POURRAIT qui ne le sont pas encore
    // et dont toutes les appelées le sont déjà (ou le deviennent).
    int nbPasses = 0;
    if (!listePourrait.empty()) {
        // Simulation des passes
        // On maintient l'état "est actuellement const (réellement ou nouvellement)"
        vector<bool> currentConst(n, false);
        for (int i = 0; i < n; ++i) {
            if (methodes[i].declaree == "const") {
                currentConst[i] = true;
            }
        }

        int totalAConvertir = listePourrait.size();
        int converties = 0;

        while (converties < totalAConvertir) {
            nbPasses++;
            vector<int> aConvertirCettePasse;

            for (int i = 0; i < n; ++i) {
                if (pourrait[i] && !currentConst[i]) {
                    bool toutesAppeleesConst = true;
                    for (const string& app : methodes[i].appels) {
                        int idxApp = nomVersIdx[app];
                        if (!currentConst[idxApp]) {
                            toutesAppeleesConst = false;
                            break;
                        }
                    }
                    if (toutesAppeleesConst) {
                        aConvertirCettePasse.push_back(i);
                    }
                }
            }

            // Si aucune méthode ne peut être convertie (sécurité, ne devrait pas arriver avec DAG valide)
            if (aConvertirCettePasse.empty()) break;

            for (int idx : aConvertirCettePasse) {
                currentConst[idx] = true;
                converties++;
            }
        }
    }

    // Affichage des résultats
    cout << "POURRAIT";
    if (listePourrait.empty()) {
        cout << " AUCUN\n";
    } else {
        for (const string& s : listePourrait) {
            cout << " " << s;
        }
        cout << "\n";
    }

    cout << "RACINES";
    if (listeRacines.empty()) {
        cout << " AUCUN\n";
    } else {
        for (const string& s : listeRacines) {
            cout << " " << s;
        }
        cout << "\n";
    }

    cout << "PASSES " << nbPasses << "\n";

    return 0;
}