#pragma once
#include "Tile.h"
#include "MineTile.h"
#include <vector>
#include <memory>
class GameState {
public:
    enum PlayStatus { WIN, LOSS, PLAYING };     //state of the current game
    GameState(sf::Vector2i _dimensions = sf::Vector2i(25, 16), int _numberOfMines = 50);        //constructor with default values
    GameState(const char* filepath);        //constructor when load dimension and board from another file
    int getFlagCount();     //return the current flagged tile on the board
    int getMineCount();     //return the total number of mines
    Tile* getTile(int x, int y);        //return the tile at a specific position or nullptr if there is no tile at that position
    PlayStatus getPlayStatus();     //return the state of game state
    void setPlayStatus(PlayStatus _status);     //set the state of game state
    ~GameState();       //destructor
private:
    PlayStatus gameStatus;
    sf::Vector2i dimensions;
    int numberOfMines;
    std::vector<std::vector<Tile*>> gameBoard;
};