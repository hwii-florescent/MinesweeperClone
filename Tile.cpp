#include "Tile.h"
#include "Toolbox.h"
#include <iostream>
Tile::Tile(sf::Vector2f position) : position(position), state(HIDDEN) {     //load all the tile texture
    mineTexture.loadFromFile("images/mine.png");
    tileHiddenTexture.loadFromFile("images/tile_hidden.png");
    tileRevealedTexture.loadFromFile("images/tile_revealed.png");
    flagTexture.loadFromFile("images/flag.png");
}

sf::Vector2f Tile::getLocation() {
    return this->position;
}

Tile::State Tile::getState() {
    return this->state;
}

std::array<Tile *, 8> &Tile::getNeighbors() {
    return this->neighbors;
}

void Tile::setState(Tile::State _state) {
    state = _state;
}

void Tile::setNeighbors(std::array<Tile *, 8> _neighbors) {
    neighbors = _neighbors;
}

void Tile::onClickLeft() {      //if tile is HIDDEN, change its state to REVEALED and call revealNeighbors
    if (state == HIDDEN) {
        state = REVEALED;
        this->revealNeighbors();
    }
}

void Tile::onCLickRight() {     //if tile is HIDDEN, change its state to FLAGGED and vice versa
    if (state == FLAGGED) {
        state = HIDDEN;
    } else if (state == HIDDEN) {
        state = FLAGGED;
    }
}

void Tile::draw() {
    Toolbox& toolbox = Toolbox::getInstance();
    switch (state) {        //render the tile based on different state
        case REVEALED:
            int count;
            for (Tile* tile : neighbors) {      //check the number of mine tiles in this tile's neighbor
                MineTile* ptr = dynamic_cast<MineTile*>(tile);
                if (ptr != nullptr) {
                    ++count;
                }
            }
            sprite.setTexture(tileRevealedTexture);
            sprite.setPosition(position);
            toolbox.window.draw(sprite);
            if (count > 0) {        //check tile's neighbor to display the number of mines around this tile
                numberTexture.loadFromFile("images/number_" + std::to_string(count) + ".png");
                sprite.setTexture(numberTexture);
                toolbox.window.draw(sprite);
            }
            break;
        case HIDDEN:
            sprite.setTexture(tileHiddenTexture);
            sprite.setPosition(position);
            toolbox.window.draw(sprite);
            break;
        case FLAGGED:
            sprite.setTexture(tileHiddenTexture);
            sprite.setPosition(position);
            toolbox.window.draw(sprite);
            if (toolbox.debugMode) {        //in debug mode, if a flag is placed on the mine tile, show the mine instead of the flag
                MineTile* ptr = dynamic_cast<MineTile*>(this);
                if (ptr == nullptr) {       //if tile is not a mine tile then show flag as normal
                    sprite.setTexture(flagTexture);
                    toolbox.window.draw(sprite);
                }
            } else {
                sprite.setTexture(flagTexture);
                toolbox.window.draw(sprite);
            }
            break;
        case EXPLODED:
            sprite.setTexture(tileRevealedTexture);
            sprite.setPosition(position);
            toolbox.window.draw(sprite);
            sprite.setTexture(mineTexture);
            toolbox.window.draw(sprite);
            toolbox.gameState->setPlayStatus(GameState::LOSS);
            break;
    }
}

void Tile::revealNeighbors() {
    bool reveal = true;
    for (Tile* tile : neighbors) {      //if tile's neighbor has no mine tile then call the onCLickLeft function for neighbors to reveal neighbors and check reveal again
        MineTile* ptr = dynamic_cast<MineTile*>(tile);
        if (ptr != nullptr) {
            reveal = false;
        }
    }
    if (reveal) {
        for (Tile* tile : neighbors) {
            if (tile != nullptr) {
                tile->onClickLeft();
            }
        }
    }
}
