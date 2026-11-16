#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
    int n = 0;
    std::cin >> n;

    // La planche : les identifiants des images, dans l'ordre (l'ordre est la profondeur)
    std::vector<int> planche;

    int prochainId = 1;     // un identifiant n'est jamais réutilisé
    int occasions = 0;      // SUPPRIME et échanges qui ont eu lieu
    int repercutes = 0;     // gestes à répéter dans les tableaux parallèles
    int recherches = 0;     // recherches dans le dictionnaire

    for (int k = 0; k < n; k++)
    {
        std::string commande;
        std::cin >> commande;

        if (commande == "AJOUTE")
        {
            // Ajouter à la fin ne change pas l'ordre : rien ne se compte
            planche.push_back(prochainId);
            prochainId++;
        }
        else if (commande == "SUPPRIME")
        {
            int c = 0;
            std::cin >> c;

            std::vector<int> index(c);
            for (int j = 0; j < c; j++)
                std::cin >> index[j];

            // On retire du plus grand index au plus petit
            for (int j = c - 1; j >= 0; j--)
                planche.erase(planche.begin() + index[j]);

            occasions++;
            repercutes += 2 * c;
        }
        else if (commande == "MONTE" || commande == "DESCEND")
        {
            int i = 0;
            std::cin >> i;

            // La voisine : i + 1 pour MONTE, i - 1 pour DESCEND
            int voisine = i + 1;
            if (commande == "DESCEND")
                voisine = i - 1;

            // Si la voisine n'existe pas, rien ne se passe et rien ne se compte
            if (voisine < 0 || voisine >= static_cast<int>(planche.size()))
                continue;

            std::swap(planche[i], planche[voisine]);
            occasions++;
            repercutes += 2;
        }
        else if (commande == "DESSINE")
        {
            // Une recherche par image présente à cet instant
            recherches += static_cast<int>(planche.size());
        }
    }

    // L'ordre final de la planche
    std::cout << "ORDRE";
    if (planche.empty())
    {
        std::cout << " AUCUN";
    }
    else
    {
        for (size_t i = 0; i < planche.size(); i++)
            std::cout << " " << planche[i];
    }
    std::cout << std::endl;

    std::cout << "OCCASIONS " << occasions << std::endl;
    std::cout << "REPERCUTES " << repercutes << std::endl;
    std::cout << "RECHERCHES " << recherches << std::endl;

    return 0;
}