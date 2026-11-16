#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// Une conception : sa propre planche, et sa façon de tenir la sélection
struct Conception
{
    std::string nom;
    int mode = 0;                       // 0 = DANS_ITEM, 1 = PAR_INDEX, 2 = PAR_ID
    std::vector<std::string> planche;   // les noms, dans l'ordre
    std::vector<int> drapeau;           // un drapeau par image (utilisé par DANS_ITEM)
    std::set<int> index;                // index gardés (utilisé par PAR_INDEX)
    std::set<std::string> noms;         // noms gardés (utilisé par PAR_ID)
};

int taille(const Conception& c)
{
    return static_cast<int>(c.planche.size());
}

// Vrai si l'index existe dans la planche de cette conception
bool existe(const Conception& c, int i)
{
    return i >= 0 && i < taille(c);
}

// Vrai si l'image à la place i est choisie selon cette conception
bool estChoisie(const Conception& c, int i)
{
    if (c.mode == 0)
        return c.drapeau[i] != 0;
    if (c.mode == 1)
        return c.index.count(i) > 0;
    return c.noms.count(c.planche[i]) > 0;
}

// Choisit l'image i (ignoré si l'index n'existe pas)
void choisir(Conception& c, int i)
{
    if (!existe(c, i))
        return;

    if (c.mode == 0)
        c.drapeau[i] = 1;
    else if (c.mode == 1)
        c.index.insert(i);
    else
        c.noms.insert(c.planche[i]);
}

// Vide la sélection
void vider(Conception& c)
{
    for (size_t i = 0; i < c.drapeau.size(); i++)
        c.drapeau[i] = 0;
    c.index.clear();
    c.noms.clear();
}

// Les noms des images choisies, dans l'ordre de la planche
std::vector<std::string> selection(const Conception& c)
{
    std::vector<std::string> resultat;
    for (int i = 0; i < taille(c); i++)
    {
        if (estChoisie(c, i))
            resultat.push_back(c.planche[i]);
    }
    return resultat;
}

// Écrit une liste de noms, ou AUCUN si elle est vide
void ecrireListe(const std::vector<std::string>& liste)
{
    if (liste.empty())
    {
        std::cout << " AUCUN";
        return;
    }
    for (size_t i = 0; i < liste.size(); i++)
        std::cout << " " << liste[i];
}

int main()
{
    const int NB = 3;
    Conception conceptions[NB];
    conceptions[0].nom = "DANS_ITEM";
    conceptions[0].mode = 0;
    conceptions[1].nom = "PAR_INDEX";
    conceptions[1].mode = 1;
    conceptions[2].nom = "PAR_ID";
    conceptions[2].mode = 2;

    std::string commande;
    while (std::cin >> commande)
    {
        if (commande == "AJOUTE")
        {
            std::string nom;
            std::cin >> nom;
            for (int t = 0; t < NB; t++)
            {
                conceptions[t].planche.push_back(nom);
                conceptions[t].drapeau.push_back(0);
            }
        }
        else if (commande == "CLIC")
        {
            int i = 0;
            std::cin >> i;
            for (int t = 0; t < NB; t++)
            {
                // La sélection se vide d'abord, même si i n'existe pas
                vider(conceptions[t]);
                choisir(conceptions[t], i);
            }
        }
        else if (commande == "AJOUTE_SEL")
        {
            int i = 0;
            std::cin >> i;
            for (int t = 0; t < NB; t++)
                choisir(conceptions[t], i);
        }
        else if (commande == "ECHANGE")
        {
            int i = 0;
            int j = 0;
            std::cin >> i >> j;
            for (int t = 0; t < NB; t++)
            {
                Conception& c = conceptions[t];
                if (existe(c, i) && existe(c, j))
                {
                    // Les index gardés de PAR_INDEX ne changent pas
                    std::swap(c.planche[i], c.planche[j]);
                    std::swap(c.drapeau[i], c.drapeau[j]);
                }
            }
        }
        else if (commande == "RETIRE")
        {
            int i = 0;
            std::cin >> i;
            for (int t = 0; t < NB; t++)
            {
                Conception& c = conceptions[t];
                if (existe(c, i))
                {
                    c.planche.erase(c.planche.begin() + i);
                    c.drapeau.erase(c.drapeau.begin() + i);
                }
            }
        }
        else if (commande == "SUPPRIME")
        {
            for (int t = 0; t < NB; t++)
            {
                Conception& c = conceptions[t];

                // On retire du plus grand index au plus petit
                for (int i = taille(c) - 1; i >= 0; i--)
                {
                    if (estChoisie(c, i))
                    {
                        c.planche.erase(c.planche.begin() + i);
                        c.drapeau.erase(c.drapeau.begin() + i);
                    }
                }

                // Puis la sélection se vide
                vider(c);
            }
        }
    }

    // Deux lignes par conception
    for (int t = 0; t < NB; t++)
    {
        std::cout << conceptions[t].nom << " PLANCHE";
        ecrireListe(conceptions[t].planche);
        std::cout << "\n";

        std::cout << conceptions[t].nom << " SELECTION";
        ecrireListe(selection(conceptions[t]));
        std::cout << "\n";
    }

    // FAUX : les conceptions qui diffèrent de DANS_ITEM
    std::vector<std::string> fausses;
    std::vector<std::string> plancheRef = conceptions[0].planche;
    std::vector<std::string> selectionRef = selection(conceptions[0]);

    for (int t = 1; t < NB; t++)
    {
        if (conceptions[t].planche != plancheRef
            || selection(conceptions[t]) != selectionRef)
        {
            fausses.push_back(conceptions[t].nom);
        }
    }

    std::cout << "FAUX";
    ecrireListe(fausses);
    std::cout << "\n";

    return 0;
}