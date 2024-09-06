#pragma once
#include "Tile.h"
class MineTile : public Tile {
public:
    void onClickLeft() override;        //function when the mine tile is left clicked
    MineTile(sf::Vector2f position);        //constructor
};


