#include "MineTile.h"

MineTile::MineTile(sf::Vector2f position) : Tile(position) {}

void MineTile::onClickLeft() {      //if a mine tile is clicked, set the state to EXPLODED
    if (this->getState() == HIDDEN) {
        this->setState(EXPLODED);
    }
}


