#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cstdint>

// Le fichier : les octets, la position de lecture, et un drapeau
// qui devient faux dès qu'on essaie de lire un octet qui manque
struct Lecteur
{
    std::vector<unsigned int> octets;
    size_t position = 0;
    bool ok = true;
};

// Lit un u8 (0 si l'octet manque, et le fichier est alors tronqué)
unsigned int lireU8(Lecteur& l)
{
    if (l.position >= l.octets.size())
    {
        l.ok = false;
        return 0;
    }
    unsigned int valeur = l.octets[l.position];
    l.position++;
    return valeur;
}

// Lit un u32 en petit-boutiste : le premier octet lu est le poids faible
uint32_t lireU32(Lecteur& l)
{
    if (l.position + 4 > l.octets.size())
    {
        l.ok = false;
        l.position = l.octets.size();
        return 0;
    }
    uint32_t valeur = 0;
    for (int i = 0; i < 4; i++)
    {
        uint32_t octet = l.octets[l.position + i];
        valeur = valeur | (octet << (8 * i));
    }
    l.position += 4;
    return valeur;
}

// Lit un f32 : on lit quatre octets, puis on les réinterprète en float
float lireF32(Lecteur& l)
{
    uint32_t bits = lireU32(l);
    float valeur = 0.0f;
    std::memcpy(&valeur, &bits, sizeof(valeur));
    return valeur;
}

// Saute un nombre d'octets (le fichier est tronqué s'il n'y en a pas assez)
void sauter(Lecteur& l, uint32_t nombre)
{
    unsigned long long reste = l.octets.size() - l.position;
    if (static_cast<unsigned long long>(nombre) > reste)
    {
        l.ok = false;
        l.position = l.octets.size();
        return;
    }
    l.position += nombre;
}

// Écrit un nombre à deux chiffres, sans jamais écrire -0.00
std::string formater(double valeur)
{
    char tampon[64];
    std::snprintf(tampon, sizeof(tampon), "%.2f", valeur);
    std::string texte = tampon;
    if (texte == "-0.00")
        texte = "0.00";
    return texte;
}

int main()
{
    long long t = 0;
    std::cin >> t;

    // Les T octets, écrits en hexadécimal
    Lecteur l;
    std::string mot;
    while (static_cast<long long>(l.octets.size()) < t && std::cin >> mot)
    {
        l.octets.push_back(static_cast<unsigned int>(std::stoul(mot, nullptr, 16)));
    }

    // Le nombre magique : N K R F, soit 4E 4B 52 46
    unsigned int magique[4];
    for (int i = 0; i < 4; i++)
        magique[i] = lireU8(l);

    if (!l.ok)
    {
        std::cout << "TRONQUE" << std::endl;
        return 0;
    }
    if (magique[0] != 0x4E || magique[1] != 0x4B
        || magique[2] != 0x52 || magique[3] != 0x46)
    {
        std::cout << "PAS_UNE_PLANCHE" << std::endl;
        return 0;
    }

    // La version
    uint32_t version = lireU32(l);
    if (!l.ok)
    {
        std::cout << "TRONQUE" << std::endl;
        return 0;
    }
    if (version != 1)
    {
        std::cout << "VERSION_INCONNUE " << version << std::endl;
        return 0;
    }

    // On garde la sortie de côté : si le fichier est refusé,
    // on n'affiche qu'une seule ligne
    std::ostringstream sortie;

    // La vue
    float cx = lireF32(l);
    float cy = lireF32(l);
    float zoom = lireF32(l);
    sortie << "VUE " << formater(cx) << " " << formater(cy) << " "
           << formater(zoom) << "\n";

    // Le thème et la grille, puis deux octets de remplissage
    unsigned int theme = lireU8(l);
    unsigned int grille = lireU8(l);
    lireU8(l);
    lireU8(l);

    if (theme != 0)
        sortie << "THEME SOMBRE\n";
    else
        sortie << "THEME CLAIR\n";

    if (grille == 0)
        sortie << "GRILLE AFFICHEE\n";
    else
        sortie << "GRILLE CACHEE\n";

    // Les images
    uint32_t nbImages = lireU32(l);
    sortie << "IMAGES " << nbImages << "\n";

    for (uint32_t k = 1; k <= nbImages && l.ok; k++)
    {
        float x = lireF32(l);
        float y = lireF32(l);
        float echelle = lireF32(l);
        float rotation = lireF32(l);
        float opacite = lireF32(l);
        unsigned int miroirX = lireU8(l);
        unsigned int miroirY = lireU8(l);
        lireU8(l);
        lireU8(l);
        uint32_t largeur = lireU32(l);
        uint32_t hauteur = lireU32(l);
        uint32_t taille = lireU32(l);

        // Les octets source sont sautés
        sauter(l, taille);

        if (!l.ok)
            break;

        // Un miroir s'écrit 1 s'il ne vaut pas zéro, 0 sinon
        int mx = (miroirX != 0) ? 1 : 0;
        int my = (miroirY != 0) ? 1 : 0;

        sortie << "IMAGE " << k << " " << formater(x) << " " << formater(y)
               << " " << formater(echelle) << " " << formater(rotation)
               << " " << formater(opacite) << " " << mx << " " << my
               << " " << largeur << " " << hauteur << " " << taille << "\n";
    }

    // Les traits
    uint32_t nbTraits = 0;
    if (l.ok)
    {
        nbTraits = lireU32(l);
        sortie << "TRAITS " << nbTraits << "\n";
    }

    for (uint32_t k = 1; k <= nbTraits && l.ok; k++)
    {
        unsigned int r = lireU8(l);
        unsigned int g = lireU8(l);
        unsigned int b = lireU8(l);
        unsigned int a = lireU8(l);
        float epaisseur = lireF32(l);
        uint32_t nbPoints = lireU32(l);

        if (!l.ok)
            break;

        sortie << "TRAIT " << k << " " << r << " " << g << " " << b << " "
               << a << " " << formater(epaisseur) << " " << nbPoints << "\n";

        for (uint32_t p = 0; p < nbPoints && l.ok; p++)
        {
            float px = lireF32(l);
            float py = lireF32(l);
            if (!l.ok)
                break;
            sortie << "POINT " << formater(px) << " " << formater(py) << "\n";
        }
    }

    // Il a manqué un octet quelque part : une seule ligne
    if (!l.ok)
    {
        std::cout << "TRONQUE" << std::endl;
        return 0;
    }

    // Les octets qui suivent le dernier trait sont ignorés
    std::cout << sortie.str();

    return 0;
}