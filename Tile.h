#pragma once
#include <SFML/Graphics.hpp>

class Tile {
public:
    enum State { REVEALED, HIDDEN, FLAGGED, EXPLODED };     //tile's current state
    Tile(sf::Vector2f position);        //constructor
    sf::Vector2f getLocation();         //return the window position of the tile
    State getState();       //return the current state of the tile
    std::array<Tile*, 8>& getNeighbors();       //return a tile pointer array of tile's neighbor
    void setState(State _state);        //set the state for the tile
    void setNeighbors(std::array<Tile*, 8> _neighbors);         //set the neighbors for the tile
    virtual void onClickLeft();     //reveal the behavior when left mouse is clicked on tile
    void onCLickRight();        //reveal the behavior when right mouse is clicked on tile
    virtual void draw();        //render the tile to the screen
protected:
    void revealNeighbors();     //base on tile neighbors, set their state to REVEALED
private:
    sf::Vector2f position;
    State state;
    std::array<Tile*, 8> neighbors;
    sf::Texture mineTexture, tileHiddenTexture, tileRevealedTexture, flagTexture, numberTexture;
    sf::Sprite sprite;
};


