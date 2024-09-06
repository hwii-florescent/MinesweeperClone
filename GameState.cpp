#include "GameState.h"
#include <fstream>
#include <random>
#include <chrono>
#include <unordered_set>
GameState::GameState(sf::Vector2i _dimensions, int _numberOfMines) : dimensions(_dimensions), numberOfMines(_numberOfMines), gameStatus(PLAYING) {
    gameBoard.resize(dimensions.y, std::vector<Tile *>(dimensions.x));      //resize the board data to the current dimension
    auto seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());     //generate random numbers to create random mine's position
    std::mt19937 gen(seed);
    std::unordered_set<int> uniqueNumbers;
    while (uniqueNumbers.size() < numberOfMines) {      //generate numberOfMines random numbers
        std::uniform_int_distribution<int> distribution(0, dimensions.x * dimensions.y - 1);
        int randomNumber = distribution(gen);
        uniqueNumbers.insert(randomNumber);
    }
    std::vector<int> mine;      //create a vector and if the index is the same as the uniqueNumbers set, set it as 1 (has mine)
    for (int i = 0; i < dimensions.x * dimensions.y; ++i) {
        mine.push_back(0);
    }
    for (int elm: uniqueNumbers) {
        mine[elm] = 1;
    }
    for (size_t i = 0; i < dimensions.y; ++i) {     //create mine tile if the position of that tile is the same as the index of 1 in vector mine, else create tile
        for (size_t j = 0; j < dimensions.x; ++j) {
            if (mine[i * dimensions.x + j] == 1) {
                gameBoard[i][j] = new MineTile(sf::Vector2f(j * 32, i * 32));
            } else {
                gameBoard[i][j] = new Tile(sf::Vector2f(j * 32, i * 32));
            }
        }
    }
    for (size_t i = 0; i < dimensions.y; ++i) {     //loop through the board data to set the neighbors for each tile
        for (size_t j = 0; j < dimensions.x; ++j) {
            std::array<Tile *, 8> neighbor = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
            if (i > 0 && j > 0) {
                neighbor[0] = gameBoard[i - 1][j - 1];
            }
            if (i > 0) {
                neighbor[1] = gameBoard[i - 1][j];
            }
            if (i > 0 && j < dimensions.x - 1) {
                neighbor[2] = gameBoard[i - 1][j + 1];
            }
            if (j > 0) {
                neighbor[3] = gameBoard[i][j - 1];
            }
            if (j < dimensions.x - 1) {
                neighbor[4] = gameBoard[i][j + 1];
            }
            if (i < dimensions.y - 1 && j > 0) {
                neighbor[5] = gameBoard[i + 1][j - 1];
            }
            if (i < dimensions.y - 1) {
                neighbor[6] = gameBoard[i + 1][j];
            }
            if (i < dimensions.y - 1 && j < dimensions.x - 1) {
                neighbor[7] = gameBoard[i + 1][j + 1];
            }
            gameBoard[i][j]->setNeighbors(neighbor);
        }
    }
}
GameState::GameState(const char *filepath) : gameStatus(PLAYING), numberOfMines(0) {
    std::string line;       //file stream and take in all data into a 2d integer vector
    std::ifstream file(filepath);
    std::vector<std::vector<int>> board;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char &c : line) {
            row.push_back((int) c - '0');
        }
        board.push_back(row);
    }
    file.close();
    dimensions = sf::Vector2i(board[0].size(), board.size());       //set the new dimension
    gameBoard.resize(board.size(), std::vector<Tile*>(board[0].size()));        //resize board data
    for (int i = 0; i < board.size(); ++i) {        //compare the index of board data with the file data vector and set mine tile if the value is 1
        for (int j = 0; j < board[0].size(); ++j) {
            if (board[i][j] == 0) {
                gameBoard[i][j] = new Tile(sf::Vector2f (j * 32, i * 32));
            } else if (board[i][j] == 1){
                ++numberOfMines;
                gameBoard[i][j] = new MineTile(sf::Vector2f(j * 32, i * 32));
            }
        }
    }
    for (size_t i = 0; i < dimensions.y; ++i) {     //loop through the board data to set the neighbors for each tile
        for (size_t j = 0; j < dimensions.x; ++j) {
            std::array<Tile *, 8> neighbor = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
            if (i > 0 && j > 0) {
                neighbor[0] = gameBoard[i - 1][j - 1];
            }
            if (i > 0) {
                neighbor[1] = gameBoard[i - 1][j];
            }
            if (i > 0 && j < dimensions.x - 1) {
                neighbor[2] = gameBoard[i - 1][j + 1];
            }
            if (j > 0) {
                neighbor[3] = gameBoard[i][j - 1];
            }
            if (j < dimensions.x - 1) {
                neighbor[4] = gameBoard[i][j + 1];
            }
            if (i < dimensions.y - 1 && j > 0) {
                neighbor[5] = gameBoard[i + 1][j - 1];
            }
            if (i < dimensions.y - 1) {
                neighbor[6] = gameBoard[i + 1][j];
            }
            if (i < dimensions.y - 1 && j < dimensions.x - 1) {
                neighbor[7] = gameBoard[i + 1][j + 1];
            }
            gameBoard[i][j]->setNeighbors(neighbor);
        }
    }
}

int GameState::getFlagCount() {     //loop through the board data can count the amount of flags
    int count = 0;
    for (std::vector<Tile*> row : gameBoard) {
        for (Tile* tile : row) {
            if (tile->getState() == Tile::FLAGGED) {
                ++count;
            }
        }
    }
    return count;
}

int GameState::getMineCount() {
    return numberOfMines;
}

Tile *GameState::getTile(int x, int y) {
    return x >= 0 && x < dimensions.y && y >= 0 && y < dimensions.x ? gameBoard[x][y] : nullptr;
}

GameState::PlayStatus GameState::getPlayStatus() {
    return gameStatus;
}

void GameState::setPlayStatus(GameState::PlayStatus _status) {
    gameStatus = _status;
}

GameState::~GameState() {       //delete every tile of board data
    for (size_t i = 0; i < dimensions.y; ++i) {
        for (size_t j = 0; j < dimensions.x; ++j) {
            delete gameBoard[i][j];
        }
    }
}
