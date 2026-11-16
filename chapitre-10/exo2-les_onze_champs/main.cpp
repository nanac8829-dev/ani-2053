#include <iostream>
#include <string>

int main()
{
    int n = 0;
    std::cin >> n;

    int restent = 0;    // nombre d'images rouvertes
    int changes = 0;    // nombre total de champs changés

    for (int k = 1; k <= n; k++)
    {
        // Les treize mots de la ligne, même ceux qui ne changent jamais
        int id;
        double x, y, echelle, rotation, opacite;
        int miroirX, miroirY;
        int largeur, hauteur;
        int selectionne;
        std::string chemin;
        long long octets;

        std::cin >> id >> x >> y >> echelle >> rotation >> opacite
                 >> miroirX >> miroirY >> largeur >> hauteur
                 >> selectionne >> chemin >> octets;

        // Une image sans octets ne peut pas être décodée : elle est sautée
        if (octets == 0)
        {
            std::cout << "ITEM " << k << " SAUTE" << std::endl;
            continue;
        }

        // Cette image revient : elle prend la place suivante
        restent++;

        // On compte les champs qui reviennent changés
        int nbChanges = 0;
        bool idChange = (id != 0);
        bool selectedChange = (selectionne == 1);
        bool pathChange = (chemin != "-");

        if (idChange) nbChanges++;
        if (selectedChange) nbChanges++;
        if (pathChange) nbChanges++;

        changes += nbChanges;

        // Affichage : ITEM k j, puis les champs changés ou AUCUN
        std::cout << "ITEM " << k << " " << restent;
        if (nbChanges == 0)
        {
            std::cout << " AUCUN";
        }
        else
        {
            if (idChange) std::cout << " id";
            if (selectedChange) std::cout << " selected";
            if (pathChange) std::cout << " sourcePath";
        }
        std::cout << std::endl;
    }

    std::cout << "RESTENT " << restent << std::endl;
    std::cout << "CHANGES " << changes << std::endl;

    return 0;
}