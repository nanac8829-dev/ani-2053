#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum Etat { ABSENT, ECRIT, DEFAUT, SUPPRIME };

string etatToString(Etat e) {
    switch (e) {
        case ABSENT: return "ABSENT";
        case ECRIT: return "ECRIT";
        case DEFAUT: return "DEFAUT";
        case SUPPRIME: return "SUPPRIME";
    }
    return "ABSENT";
}

// Supprime tous les espaces d'une chaîne
string enleverEspaces(const string& s) {
    string res = "";
    for (char c : s) {
        if (!isspace(static_cast<unsigned char>(c))) {
            res += c;
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string nomClasse;
    if (!(cin >> nomClasse)) return 0;

    // Consommer le retour à la ligne après le nom de la classe
    string dummy;
    getline(cin, dummy);

    Etat destructeur = ABSENT;
    Etat copie = ABSENT;
    Etat affectationCopie = ABSENT;
    Etat deplacement = ABSENT;
    Etat affectationDeplacement = ABSENT;

    string ligne;
    while (getline(cin, ligne)) {
        // 1. Couper au premier //
        size_t posCommentaire = ligne.find("//");
        if (posCommentaire != string::npos) {
            ligne = ligne.substr(0, posCommentaire);
        }

        // 2. Retirer toutes les espaces
        string s = enleverEspaces(ligne);
        if (s.empty()) continue;

        // Déterminer l'état final (SUPPRIME, DEFAUT ou ECRIT)
        Etat etatLigne = ECRIT;
        if (s.size() >= 8 && s.substr(s.size() - 8) == "=delete;") {
            etatLigne = SUPPRIME;
        } else if (s.size() >= 9 && s.substr(s.size() - 9) == "=default;") {
            etatLigne = DEFAUT;
        }

        // Construction des motifs de reconnaissance en remplaçant C par le nom de la classe
        string sDestructeur = "~" + nomClasse + "(";
        string sCopie = nomClasse + "(const" + nomClasse + "&";
        string sAffectationCopie = nomClasse + "&operator=(const" + nomClasse + "&";
        string sDeplacement = nomClasse + "(" + nomClasse + "&&";
        string sAffectationDeplacement = nomClasse + "&operator=(" + nomClasse + "&&"; // Correction ici avec '('

        // Vérification des 5 membres spéciaux selon les règles strictes :
        if (s.rfind(sDestructeur, 0) == 0) {
            destructeur = etatLigne;
        }
        else if (s.rfind(sCopie, 0) == 0) {
            size_t idx = sCopie.length();
            if (idx < s.length() && s[idx] == '&') {
                // C'est un déplacement, pas une copie !
            } else {
                copie = etatLigne;
            }
        }
        else if (s.rfind(sAffectationCopie, 0) == 0) {
            size_t idx = sAffectationCopie.length();
            if (idx < s.length() && s[idx] == '&') {
                // C'est un déplacement par affectation
            } else {
                affectationCopie = etatLigne;
            }
        }
        else if (s.rfind(sDeplacement, 0) == 0) {
            deplacement = etatLigne;
        }
        else if (s.rfind(sAffectationDeplacement, 0) == 0) {
            affectationDeplacement = etatLigne;
        }
    }

    // Affichage des états demandés
    cout << "DESTRUCTEUR " << etatToString(destructeur) << "\n";
    cout << "COPIE " << etatToString(copie) << "\n";
    cout << "AFFECTATION_COPIE " << etatToString(affectationCopie) << "\n";
    cout << "DEPLACEMENT " << etatToString(deplacement) << "\n";
    cout << "AFFECTATION_DEPLACEMENT " << etatToString(affectationDeplacement) << "\n";

    // Analyse de la règle
    bool d_decl = (destructeur != ABSENT);
    bool c_decl = (copie != ABSENT);
    bool ac_decl = (affectationCopie != ABSENT);
    bool dep_decl = (deplacement != ABSENT);
    bool adep_decl = (affectationDeplacement != ABSENT);

    string regle = "INCOMPLETE";

    if (!d_decl && !c_decl && !ac_decl && !dep_decl && !adep_decl) {
        regle = "ZERO";
    } else if (d_decl && c_decl && ac_decl && dep_decl && adep_decl) {
        regle = "CINQ";
    } else if (d_decl && c_decl && ac_decl && !dep_decl && !adep_decl) {
        regle = "TROIS";
    } else {
        regle = "INCOMPLETE";
    }

    cout << "REGLE " << regle << "\n";

    if (regle == "INCOMPLETE") {
        vector<pair<string, bool>> membres = {
            {"DESTRUCTEUR", d_decl},
            {"COPIE", c_decl},
            {"AFFECTATION_COPIE", ac_decl},
            {"DEPLACEMENT", dep_decl},
            {"AFFECTATION_DEPLACEMENT", adep_decl}
        };

        bool au_moins_un_deplacement = dep_decl || adep_decl;

        for (size_t i = 0; i < membres.size(); ++i) {
            if (!au_moins_un_deplacement && i >= 3) {
                continue;
            }
            if (!membres[i].second) {
                cout << "MANQUE " << membres[i].first << "\n";
            }
        }
    }

    if (destructeur == ECRIT) {
        cout << "POSSEDE OUI\n";
    } else {
        cout << "POSSEDE NON\n";
    }

    if (copie == SUPPRIME && (deplacement == ECRIT || deplacement == DEFAUT)) {
        cout << "UNIQUE OUI\n";
    } else {
        cout << "UNIQUE NON\n";
    }

    return 0;
}