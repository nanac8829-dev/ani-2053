#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// Une image de la planche : son nom, son centre, sa taille native et son échelle
struct Image
{
    std::string nom;
    double x = 0.0;
    double y = 0.0;
    double largeur = 0.0;
    double hauteur = 0.0;
    double echelle = 1.0;
};

// Vrai si le point (px, py) touche l'image (le bord compte)
bool touche(const Image& image, double px, double py)
{
    double demiLargeur = image.largeur * image.echelle / 2.0;
    double demiHauteur = image.hauteur * image.echelle / 2.0;

    return std::fabs(px - image.x) <= demiLargeur
        && std::fabs(py - image.y) <= demiHauteur;
}

int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<Image> planche;
    for (int i = 0; i < n; i++)
    {
        Image image;
        std::cin >> image.nom >> image.x >> image.y
                 >> image.largeur >> image.hauteur >> image.echelle;
        planche.push_back(image);
    }

    int q = 0;
    std::cin >> q;

    int erreurs = 0;

    for (int k = 1; k <= q; k++)
    {
        double px, py;
        std::cin >> px >> py;

        // A : recherche à rebours, de la dernière image à la première
        std::string a = "AUCUN";
        for (int i = n - 1; i >= 0; i--)
        {
            if (touche(planche[i], px, py))
            {
                a = planche[i].nom;
                break;
            }
        }

        // B : recherche à l'endroit, de la première image à la dernière
        std::string b = "AUCUN";
        for (int i = 0; i < n; i++)
        {
            if (touche(planche[i], px, py))
            {
                b = planche[i].nom;
                break;
            }
        }

        if (a != b)
            erreurs++;

        std::cout << "CLIC " << k << " " << a << " " << b << std::endl;
    }

    std::cout << "ERREURS " << erreurs << std::endl;

    return 0;
}