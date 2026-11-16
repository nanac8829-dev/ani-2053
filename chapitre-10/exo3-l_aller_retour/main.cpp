#include <iostream>
#include <string>
#include <cstdio>

// La caméra : seulement le centre et le zoom (pas la taille de la fenêtre)
struct Camera
{
    double cx = 0.0;
    double cy = 0.0;
    double zoom = 1.0;
};

// Écrit un nombre avec le bon nombre de chiffres, sans jamais écrire -0.00
std::string formater(double valeur, int chiffres)
{
    char tampon[64];
    if (chiffres == 6)
        std::snprintf(tampon, sizeof(tampon), "%.6f", valeur);
    else
        std::snprintf(tampon, sizeof(tampon), "%.2f", valeur);

    std::string texte = tampon;

    // Une petite valeur négative arrondie à zéro s'écrit sans le signe moins
    if (texte == "-0.00")
        texte = "0.00";
    if (texte == "-0.000000")
        texte = "0.000000";

    return texte;
}

// Pixel vers monde
void versMonde(const Camera& cam, double largeur, double hauteur,
               double px, double py, double& mx, double& my)
{
    mx = cam.cx + (px - largeur / 2.0) / cam.zoom;
    my = cam.cy + (py - hauteur / 2.0) / cam.zoom;
}

// Monde vers pixel
void versPixel(const Camera& cam, double largeur, double hauteur,
               double mx, double my, double& px, double& py)
{
    px = (mx - cam.cx) * cam.zoom + largeur / 2.0;
    py = (my - cam.cy) * cam.zoom + hauteur / 2.0;
}

// Déplacement : glisser la planche vers la droite fait reculer le centre
void deplacer(Camera& cam, double dx, double dy)
{
    cam.cx = cam.cx - dx / cam.zoom;
    cam.cy = cam.cy - dy / cam.zoom;
}

// Zoom au curseur : le point du monde sous le curseur ne bouge pas
void zoomer(Camera& cam, double largeur, double hauteur,
            double f, double px, double py)
{
    // 1. On note le point du monde sous le curseur AVANT de changer le zoom
    double ax, ay;
    versMonde(cam, largeur, hauteur, px, py, ax, ay);

    // 2. On change le zoom, puis on le borne entre 0.02 et 64
    cam.zoom = cam.zoom * f;
    if (cam.zoom < 0.02)
        cam.zoom = 0.02;
    if (cam.zoom > 64.0)
        cam.zoom = 64.0;

    // 3. On replace le centre pour que le point retombe sur le même pixel
    cam.cx = ax - (px - largeur / 2.0) / cam.zoom;
    cam.cy = ay - (py - hauteur / 2.0) / cam.zoom;
}

// Pas de la grille : lignes écartées d'au moins m pixels
double pasGrille(const Camera& cam, double m)
{
    double s = 1.0;

    while (s * cam.zoom < m)
        s = s * 2.0;

    while (s * 0.5 * cam.zoom >= m)
        s = s / 2.0;

    return s;
}

int main()
{
    double largeur = 0.0;
    double hauteur = 0.0;
    std::cin >> largeur >> hauteur;

    Camera cam;

    std::string commande;
    while (std::cin >> commande)
    {
        if (commande == "VERS_MONDE")
        {
            double px, py, mx, my;
            std::cin >> px >> py;
            versMonde(cam, largeur, hauteur, px, py, mx, my);
            std::cout << "MONDE " << formater(mx, 2) << " " << formater(my, 2) << std::endl;
        }
        else if (commande == "VERS_PIXEL")
        {
            double mx, my, px, py;
            std::cin >> mx >> my;
            versPixel(cam, largeur, hauteur, mx, my, px, py);
            std::cout << "PIXEL " << formater(px, 2) << " " << formater(py, 2) << std::endl;
        }
        else if (commande == "DEPLACE")
        {
            double dx, dy;
            std::cin >> dx >> dy;
            deplacer(cam, dx, dy);
        }
        else if (commande == "ZOOM")
        {
            double f, px, py;
            std::cin >> f >> px >> py;
            zoomer(cam, largeur, hauteur, f, px, py);
        }
        else if (commande == "GRILLE")
        {
            double m;
            std::cin >> m;
            std::cout << "GRILLE " << formater(pasGrille(cam, m), 6) << std::endl;
        }
        else if (commande == "ORIGINE")
        {
            cam.cx = 0.0;
            cam.cy = 0.0;
            cam.zoom = 1.0;
        }
    }

    // Toujours affichés, même sans aucune commande
    std::cout << "CENTRE " << formater(cam.cx, 2) << " " << formater(cam.cy, 2) << std::endl;
    std::cout << "ZOOM " << formater(cam.zoom, 2) << std::endl;

    return 0;
}