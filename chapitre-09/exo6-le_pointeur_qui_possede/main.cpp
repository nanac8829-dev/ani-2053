#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

struct LigneInst {
    int num;
    string cmd;
    string arg1;
    string arg2;
};

int main() {
    // Optimisation des entrées-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<LigneInst> instructions(N);
    vector<int> lignes_copie;

    for (int i = 0; i < N; ++i) {
        instructions[i].num = i + 1;
        cin >> instructions[i].cmd;
        if (instructions[i].cmd == "NEW" || instructions[i].cmd == "RESET" || instructions[i].cmd == "USE") {
            cin >> instructions[i].arg1;
        } else if (instructions[i].cmd == "MOVE" || instructions[i].cmd == "COPY") {
            cin >> instructions[i].arg1 >> instructions[i].arg2;
        }
        
        if (instructions[i].cmd == "COPY") {
            lignes_copie.push_back(i + 1);
        }
    }

    // Piège : Si une ligne COPY existe, le programme ne compile pas et affiche seulement REFUSE k
    if (!lignes_copie.empty()) {
        for (int k : lignes_copie) {
            cout << "REFUSE " << k << "\n";
        }
        return 0;
    }

    map<string, int> variables; // Nom de la variable -> ID de l'objet possédé (0 si nul)
    vector<string> ordre_apparition; // Pour suivre l'ordre de la première apparition des variables

    auto enregistrer_variable = [&](const string& var) {
        if (variables.find(var) == variables.end()) {
            variables[var] = 0;
            ordre_apparition.push_back(var);
        }
    };

    int total_crees = 0;
    int total_detruits = 0;
    int total_nuls = 0;

    // Fonction pour détruire un objet s'il existe
    auto detruire_objet = [&](int objet_id) {
        if (objet_id != 0) {
            cout << "DETRUIT " << objet_id << "\n";
            total_detruits++;
        }
    };

    // Exécution des instructions
    for (const auto& inst : instructions) {
        if (inst.cmd == "NEW") {
            string a = inst.arg1;
            enregistrer_variable(a);
            total_crees++;
            int nouvel_objet = total_crees;

            // Si a tenait déjà un objet, il meurt
            detruire_objet(variables[a]);
            variables[a] = nouvel_objet;

        } else if (inst.cmd == "MOVE") {
            string a = inst.arg1;
            string b = inst.arg2;
            enregistrer_variable(a);
            enregistrer_variable(b);

            int objet_a = variables[a];
            variables[a] = 0; // a devient nul

            // Si b tenait un objet, il meurt
            detruire_objet(variables[b]);
            variables[b] = objet_a; // b tient ce que a tenait (même si 0)

        } else if (inst.cmd == "RESET") {
            string a = inst.arg1;
            enregistrer_variable(a);

            if (variables[a] != 0) {
                detruire_objet(variables[a]);
                variables[a] = 0;
            }

        } else if (inst.cmd == "USE") {
            string a = inst.arg1;
            enregistrer_variable(a);

            if (variables[a] == 0) {
                cout << "NUL " << inst.num << " " << a << "\n";
                total_nuls++;
            }
        }
    }

    // Fin du programme : les variables meurent dans l'ordre inverse de leur première apparition
    for (auto it = ordre_apparition.rbegin(); it != ordre_apparition.rend(); ++it) {
        const string& var = *it;
        if (variables[var] != 0) {
            detruire_objet(variables[var]);
            variables[var] = 0;
        }
    }

    // Affichage final des statistiques
    cout << "CREES " << total_crees << "\n";
    cout << "DETRUITS " << total_detruits << "\n";
    cout << "NULS " << total_nuls << "\n";

    return 0;
}