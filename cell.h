#include <SFML/Graphics.hpp>

#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    Cell();
    ~Cell();
    int num;
    int stat; // 0 - closed     1 - opened      2 - flaged
    bool isMine;
    sf::Sprite sprite;
};

#endif