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

    string dummy;
    getline(cin, dummy);

    Etat destructeur = ABSENT;
    Etat copie = ABSENT;
    Etat affectationCopie = ABSENT;
    Etat deplacement = ABSENT;
    Etat affectationDeplacement = ABSENT;

    string ligne;
    while (getline(cin, ligne)) {
        size_t posCommentaire = ligne.find("//");
        if (posCommentaire != string::npos) {
            ligne = ligne.substr(0, posCommentaire);
        }

        string s = enleverEspaces(ligne);
        if (s.empty()) continue;

        Etat etatLigne = ECRIT;
        if (s.size() >= 8 && s.substr(s.size() - 8) == "=delete;") {
            etatLigne = SUPPRIME;
        } else if (s.size() >= 9 && s.substr(s.size() - 9) == "=default;") {
            etatLigne = DEFAUT;
        }

        string sDestructeur = "~" + nomClasse + "(";
        string sCopie = nomClasse + "(const" + nomClasse + "&";
        string sAffectationCopie = nomClasse + "&operator=(const" + nomClasse + "&";
        string sDeplacement = nomClasse + "(" + nomClasse + "&&";
        string sAffectationDeplacement = nomClasse + "&operator=" + nomClasse + "&&";

        if (s.rfind(sDestructeur, 0) == 0) {
            destructeur = etatLigne;
        } else if (s.rfind(sCopie, 0) == 0) {
            size_t idx = sCopie.length();
            if (!(idx < s.length() && s[idx] == '&')) {
                copie = etatLigne;
            }
        } else if (s.rfind(sAffectationCopie, 0) == 0) {
            size_t idx = sAffectationCopie.length();
            if (!(idx < s.length() && s[idx] == '&')) {
                affectationCopie = etatLigne;
            }
        } else if (s.rfind(sDeplacement, 0) == 0) {
            deplacement = etatLigne;
        } else if (s.rfind(sAffectationDeplacement, 0) == 0) {
            affectationDeplacement = etatLigne;
        }
    }

    // Affichage strict des 5 lignes demandées
    cout << "DESTRUCTEUR " << etatToString(destructeur) << "\n";
    cout << "COPIE " << etatToString(copie) << "\n";
    cout << "AFFECTATION_COPIE " << etatToString(affectationCopie) << "\n";
    cout << "DEPLACEMENT " << etatToString(deplacement) << "\n";
    cout << "AFFECTATION_DEPLACEMENT " << etatToString(affectationDeplacement) << "\n";

    return 0;
}