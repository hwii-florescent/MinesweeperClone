#include "Button.h"
#include "GameState.h"

class Toolbox {
public:
    sf::RenderWindow window;    //SFML application window
    GameState* gameState;       //Primary game state representation
    Button* debugButton;        //Reveals mines in debug mode
    Button* newGameButton;      //Resets / starts new game
    Button* testButton1;        //Loads test board #1
    Button* testButton2;        //Loads test board #2
    Button* testButton3;        //Loads test board #3
    bool debugMode;             //Variable to check if debug mode is on

    static Toolbox& getInstance();      //returns a reference to the singular Toolbox instance
private:
    Toolbox();      //default constructor
};


