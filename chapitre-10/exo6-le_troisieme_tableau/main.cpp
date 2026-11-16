#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Les trois tableaux : 0 = modèle, 1 = textures, 2 = sources
const int NB_TABLEAUX = 3;

// Taille d'un tableau, en entier
int taille(const std::vector<std::string>& tableau)
{
    return static_cast<int>(tableau.size());
}

// Lit une ligne de réglage : le premier mot est la commande,
// les mots suivants sont les tableaux cités (textures, sources)
void lireReglage(const std::string& ligne, bool ajoute[], bool supprime[], bool echange[])
{
    std::istringstream flux(ligne);
    std::string commande;
    flux >> commande;

    bool* cible = nullptr;
    if (commande == "AJOUTE")
        cible = ajoute;
    else if (commande == "SUPPRIME")
        cible = supprime;
    else if (commande == "ECHANGE")
        cible = echange;

    if (cible == nullptr)
        return;

    std::string nom;
    while (flux >> nom)
    {
        if (nom == "textures")
            cible[1] = true;
        else if (nom == "sources")
            cible[2] = true;
    }
}

int main()
{
    // Le modèle est toujours touché : seuls textures et sources sont réglables
    bool ajoute[NB_TABLEAUX] = { true, false, false };
    bool supprime[NB_TABLEAUX] = { true, false, false };
    bool echange[NB_TABLEAUX] = { true, false, false };

    // Les trois lignes de réglage (elles peuvent ne citer aucun nom)
    std::string ligne;
    for (int i = 0; i < 3; i++)
    {
        std::getline(std::cin, ligne);
        lireReglage(ligne, ajoute, supprime, echange);
    }

    int n = 0;
    std::cin >> n;

    // Les trois tableaux partent vides
    std::vector<std::string> tableaux[NB_TABLEAUX];

    for (int k = 0; k < n; k++)
    {
        std::string commande;
        std::cin >> commande;

        if (commande == "AJOUTE")
        {
            std::string nom;
            std::cin >> nom;

            // Le nom entre à la fin des tableaux cités
            for (int t = 0; t < NB_TABLEAUX; t++)
            {
                if (ajoute[t])
                    tableaux[t].push_back(nom);
            }
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
            {
                for (int t = 0; t < NB_TABLEAUX; t++)
                {
                    if (!supprime[t])
                        continue;

                    // Un index qui n'existe pas dans ce tableau est sauté
                    if (index[j] < taille(tableaux[t]))
                        tableaux[t].erase(tableaux[t].begin() + index[j]);
                }
            }
        }
        else if (commande == "MONTE" || commande == "DESCEND")
        {
            int i = 0;
            std::cin >> i;

            // La voisine : i + 1 pour MONTE, i - 1 pour DESCEND
            int voisine = i + 1;
            if (commande == "DESCEND")
                voisine = i - 1;

            // Si la voisine n'existe pas dans le modèle, rien ne se passe
            if (voisine < 0 || voisine >= taille(tableaux[0]))
                continue;

            // Chaque tableau cité fait le même échange, s'il a les deux index
            for (int t = 0; t < NB_TABLEAUX; t++)
            {
                if (!echange[t])
                    continue;

                if (i < taille(tableaux[t]) && voisine < taille(tableaux[t]))
                    std::swap(tableaux[t][i], tableaux[t][voisine]);
            }
        }
    }

    // L : la longueur du plus long des trois tableaux
    int longueur = 0;
    for (int t = 0; t < NB_TABLEAUX; t++)
    {
        if (taille(tableaux[t]) > longueur)
            longueur = taille(tableaux[t]);
    }

    int decales = 0;

    for (int i = 0; i < longueur; i++)
    {
        std::string noms[NB_TABLEAUX];
        for (int t = 0; t < NB_TABLEAUX; t++)
        {
            // Une case qui n'existe pas s'écrit -
            if (i < taille(tableaux[t]))
                noms[t] = tableaux[t][i];
            else
                noms[t] = "-";
        }

        std::cout << i << " " << noms[0] << " " << noms[1] << " " << noms[2] << std::endl;

        // Décalée : une case manque, ou les noms ne sont pas tous les mêmes
        bool decale = false;
        if (noms[0] == "-" || noms[1] == "-" || noms[2] == "-")
            decale = true;
        if (noms[0] != noms[1] || noms[0] != noms[2])
            decale = true;

        if (decale)
            decales++;
    }

    std::cout << "DECALES " << decales << std::endl;

    return 0;
}