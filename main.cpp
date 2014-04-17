#include <iostream>
#include <limits>
#include <map>
#include <time.h>
#include <SFML/Graphics.hpp>
#define HAUT 0
#define DROITE 1
#define BAS 2
#define GAUCHE 3

struct caseLaby
{
    std::pair<int, int> coor;
    bool state;
    caseLaby* parent;
    caseLaby* caseCote[4];
};

std::map<std::pair<int, int>,caseLaby> labyMap;

sf::Image laby;

void initLaby()
{
    for(unsigned int i = 0; i < laby.getWidth(); ++i)
    {
        for(unsigned int j = 0; j < laby.getHeight(); ++j)
        {
            if((i % 2 == 0) && (j % 2 == 0))
            {
                caseLaby caseActu;
                caseActu.coor = std::pair<int, int>(i, j);
                caseActu.state = false;
                caseActu.parent = NULL;
                labyMap[std::pair<int, int>(i, j)] = caseActu;
                continue;
            }
            else
                laby.setPixel(i, j, sf::Color::Black);
        }
    }
}

void initCase()
{
    std::map<std::pair<int, int>, caseLaby>::iterator ite = labyMap.begin();
    while(ite != labyMap.end())
    {
        if(ite->second.coor.first != 0)
            ite->second.caseCote[GAUCHE] = &labyMap[std::pair<int, int>(ite->second.coor.first - 2, ite->second.coor.second)];
        else
            ite->second.caseCote[GAUCHE] = NULL;

        if(ite->second.coor.second != 0)
            ite->second.caseCote[HAUT] = &labyMap[std::pair<int, int>(ite->second.coor.first, ite->second.coor.second - 2)];
        else
            ite->second.caseCote[HAUT] = NULL;

        if(ite->second.coor.first != (int)laby.getWidth() - 1)
            ite->second.caseCote[DROITE] = &labyMap[std::pair<int, int>(ite->second.coor.first + 2, ite->second.coor.second)];
        else
            ite->second.caseCote[DROITE] = NULL;

        if(ite->second.coor.second != (int)laby.getHeight() - 1)
            ite->second.caseCote[BAS] = &labyMap[std::pair<int, int>(ite->second.coor.first, ite->second.coor.second + 2)];
        else
            ite->second.caseCote[BAS] = NULL;

        ++ite;
    }
}

void generLaby()
{
    int numCase = rand() % labyMap.size();
    std::map<std::pair<int, int>, caseLaby>::iterator ite = labyMap.begin();
    caseLaby* caseActu;
    caseLaby* oldCase = NULL;
    bool continuer;
    int direction;
    while(numCase != 0)
    {
        ++ite;
        --numCase;
    }
    caseActu = &ite->second;
    do
    {
        continuer = false;
        caseActu->state = true;
        for(int i = 0; i < 4; ++i)
        {
            if(caseActu->caseCote[i] != NULL)
            {
                if(!caseActu->caseCote[i]->state)
                {
                    continuer = true;
                    break;
                }
            }
        }
        if(continuer)
        {
            oldCase = caseActu;
            do
            {
                do
                {
                    direction = rand() % 4;
                    caseActu = oldCase->caseCote[direction];
                }
                while(caseActu == NULL);
            }
            while(caseActu->state);

            caseActu->parent = oldCase;
            if(direction == HAUT)
                laby.setPixel(oldCase->coor.first, oldCase->coor.second - 1, sf::Color::White);

            else if(direction == BAS)
                laby.setPixel(oldCase->coor.first, oldCase->coor.second + 1, sf::Color::White);

            else if(direction == DROITE)
                laby.setPixel(oldCase->coor.first + 1, oldCase->coor.second, sf::Color::White);

            else
                laby.setPixel(oldCase->coor.first - 1, oldCase->coor.second, sf::Color::White);
        }
        else
        {
            caseActu = caseActu->parent;
        }
    }
    while(caseActu->parent != NULL);
}

int main()
{
    std::string choix, nomFichier, quitter;
    unsigned int seed = 0;
    int taille = 0;
    std::cout << "Voulez vous choisir un nombre pour initialiser la seed ou voulez-vous que le hasard fasse les choses ? y/Y pour initialiser manuellement, n/N pour laisser faire le hasard : ";
    do
    {
        std::cin >> choix;
    }
    while(choix != "y" && choix != "Y" && choix != "n" && choix != "N");
    if(choix == "y" || choix == "Y")
    {
        std::cout << "Entrez un nombre : ";
        while(!(std::cin >> seed))
        {
            std::cout << "Entrez un nombre : ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    else
    {
        seed = time(NULL);
    }
    srand(seed);
    std::cout << "Entrez le cote du carre, cela doit etre un nombre impaire : ";
    do
    {
        while(!(std::cin >> taille))
        {
            std::cout << "Entrez le cote du carre, cela doit etre un nombre impaire : ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    while(taille % 2 != 1);
    laby.create(taille, taille, sf::Color::White);
    initLaby();
    initCase();
    generLaby();
    std::cout << "Enregistrer sous : ";
    std::cin >> nomFichier;
    laby.saveToFile(nomFichier);
    std::cout << "Entrez q ou Q pour quitter : ";
    do
    {
        std::cin >> quitter;
    }
    while (quitter != "q" && quitter != "Q");
    return 0;
}
