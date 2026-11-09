#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    // Optimisation des entrées-sorties
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int S, V, F;
    if (!(cin >> S >> V >> F)) return 0;

    int N;
    if (!(cin >> N)) return 0;

    vector<int> active_ids;       // IDs des textures actuellement dans le tableau
    vector<bool> freed_status;    // État de libération de chaque texture (index = ID - 1)
    int total_created = 0;

    // Fonction lambda pour libérer une texture n fois
    auto release_texture = [&](int id, int count, const string& k_str) -> bool {
        for (int i = 0; i < count; ++i) {
            if (freed_status[id - 1]) {
                // Double libération -> Plantage immédiat
                cout << "PLANTAGE " << k_str << " TEXTURE " << id << "\n";
                return false;
            }
            freed_status[id - 1] = true;
        }
        return true;
    };

    for (int k = 1; k <= N; ++k) {
        string op;
        cin >> op;
        if (op == "AJOUTE") {
            total_created++;
            active_ids.push_back(total_created);
            freed_status.push_back(false);
        } else if (op == "SUPPRIME") {
            int idx;
            cin >> idx;
            int id = active_ids[idx];
            // Libère S fois avant de retirer du tableau
            if (!release_texture(id, S, to_string(k))) {
                return 0;
            }
            active_ids.erase(active_ids.begin() + idx);
        } else if (op == "VIDE") {
            for (int id : active_ids) {
                if (!release_texture(id, V, to_string(k))) {
                    return 0;
                }
            }
            active_ids.clear();
        }
    }

    // Fin du programme : libère F fois chaque texture encore dans le tableau
    for (int id : active_ids) {
        if (!release_texture(id, F, "FIN")) {
            return 0;
        }
    }

    // Calcul des statistiques si aucun plantage
    int fuites_count = 0;
    vector<int> perdues;
    for (int i = 0; i < total_created; ++i) {
        if (!freed_status[i]) {
            fuites_count++;
            perdues.push_back(i + 1);
        }
    }

    cout << "CREEES " << total_created << "\n";
    cout << "FUITES " << fuites_count << "\n";
    cout << "PERDUES";
    if (perdues.empty()) {
        cout << " AUCUN\n";
    } else {
        for (int id : perdues) {
            cout << " " << id;
        }
        cout << "\n";
    }

    return 0;
}