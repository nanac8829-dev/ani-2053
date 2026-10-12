#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

using ll = long long;

struct Police {
    string nom;
    ll n, w, h;
    ll x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll P, L;
    cin >> P >> L;

    int G;
    cin >> G;

    if (G == 0) {
        cout << "AUCUN\n";
        return 0;
    }

    vector<Police> polices(G);

    ll besoin = 0;
    ll occupe = 0;

    for (int i = 0; i < G; i++) {
        cin >> polices[i].nom
            >> polices[i].n
            >> polices[i].w
            >> polices[i].h;

        besoin += polices[i].n
                * (polices[i].w + P)
                * (polices[i].h + P);

        occupe += polices[i].n
                * polices[i].w
                * polices[i].h;
    }

    ll W = L;

    if (W == 0) {
        W = 512;

        while (W * W < 2 * besoin && W < 4096) {
            W *= 2;
        }
    }

    ll H = W;
    bool reussi = false;
    int essais = 0;

    for (int tentative = 1; tentative <= 8; tentative++) {
        essais = tentative;

        ll x = P;
        ll y = P;
        ll e = 0;
        bool possible = true;

        for (int i = 0; i < G && possible; i++) {
            ll rw = polices[i].w + P;
            ll rh = polices[i].h + P;

            for (ll j = 0; j < polices[i].n; j++) {
                if (x + rw > W - P) {
                    x = P;
                    y = y + e + P;
                    e = 0;
                }

                if (x + rw > W - P || y + rh > H - P) {
                    possible = false;
                    break;
                }

                if (j == 0) {
                    polices[i].x1 = x;
                    polices[i].y1 = y;
                }

                polices[i].x2 = x;
                polices[i].y2 = y;

                x += rw;
                e = max(e, rh);
            }
        }

        if (possible) {
            reussi = true;
            break;
        }

        if (tentative < 8) {
            if (W == H) {
                W *= 2;
            } else {
                H = W;
            }
        }
    }

    cout << "ESSAIS " << essais << '\n';

    if (!reussi) {
        cout << "ECHEC\n";
        return 0;
    }

    cout << "TEXTURE " << W << ' ' << H << '\n';

    for (const auto& p : polices) {
        cout << p.nom << ' '
             << p.x1 << ' ' << p.y1 << ' '
             << p.x2 << ' ' << p.y2 << '\n';
    }

    ll surface = W * H;
    ll perdu = (surface - occupe) * 100 / surface;

    cout << "OCCUPE " << occupe << '\n';
    cout << "PERDU " << perdu << '\n';

    return 0;
}