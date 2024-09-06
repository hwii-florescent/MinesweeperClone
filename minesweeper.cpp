#include "minesweeper.h"
#include "Toolbox.h"
#include <iostream>
#include <SFML/Graphics.hpp>
int launch() {
    Toolbox& toolbox = Toolbox::getInstance();
    toolbox.window.create(sf::VideoMode(800, 600), "P4 - Minesweeper, <Huy Hoang Gia>");        //create a window of desire dimension and title
    toolbox.window.setVerticalSyncEnabled(true);

    //create new game state and all the buttons and set the onclick function for each
    toolbox.gameState = new GameState(sf::Vector2i(25, 16), 50);
    toolbox.newGameButton = new Button(sf::Vector2f(400.f, 512.f), []() {       //restart the game onclick
        restart();
    });
    toolbox.debugButton = new Button(sf::Vector2f(528.f, 512.f), []() {         //flip the debug mode onclick
        toggleDebugMode();
    });
    toolbox.testButton1 = new Button(sf::Vector2f(592.f, 512.f), [&toolbox]() {         //load test 1 onclick
        toolbox.debugMode = false;
        delete toolbox.gameState;
        toolbox.gameState = new GameState("boards/testboard1.brd");
    });
    toolbox.testButton2 = new Button(sf::Vector2f(656.f, 512.f), [&toolbox]() {         //load test 2 onclick
        toolbox.debugMode = false;
        delete toolbox.gameState;
        toolbox.gameState = new GameState("boards/testboard2.brd");
    });
    toolbox.testButton3 = new Button(sf::Vector2f(720.f, 512.f), [&toolbox]() {         //load test 3 onclick
        toolbox.debugMode = false;
        delete toolbox.gameState;
        toolbox.gameState = new GameState("boards/testboard3.brd");
    });

    //load the texture for all buttons
    sf::Texture faceHappyTexture, faceWinTexture, faceLoseTexture, debugTexture, test1Texture, test2Texture, test3Texture;
    faceHappyTexture.loadFromFile("images/face_happy.png");
    faceWinTexture.loadFromFile("images/face_win.png");
    faceLoseTexture.loadFromFile("images/face_lose.png");
    debugTexture.loadFromFile("images/debug.png");
    test1Texture.loadFromFile("images/test_1.png");
    test2Texture.loadFromFile("images/test_2.png");
    test3Texture.loadFromFile("images/test_3.png");

    //create the default sprite for each button
    sf::Sprite faceSprite(faceHappyTexture);
    sf::Sprite debugSprite(debugTexture);
    sf::Sprite test1Sprite(test1Texture);
    sf::Sprite test2Sprite(test2Texture);
    sf::Sprite test3Sprite(test3Texture);

    //set the positon on screen and set the sprite for each button
    toolbox.newGameButton->setSprite(&faceSprite);
    toolbox.newGameButton->getSprite()->setPosition(toolbox.newGameButton->getPosition());
    toolbox.debugButton->setSprite(&debugSprite);
    toolbox.debugButton->getSprite()->setPosition(toolbox.debugButton->getPosition());
    toolbox.testButton1->setSprite(&test1Sprite);
    toolbox.testButton1->getSprite()->setPosition(toolbox.testButton1->getPosition());
    toolbox.testButton2->setSprite(&test2Sprite);
    toolbox.testButton2->getSprite()->setPosition(toolbox.testButton2->getPosition());
    toolbox.testButton3->setSprite(&test3Sprite);
    toolbox.testButton3->getSprite()->setPosition(toolbox.testButton3->getPosition());

    while (toolbox.window.isOpen()) {       //run the program when the window is open
        sf::Event event;
        while (toolbox.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {      //check if window is closed
                toolbox.window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed) {       //check if mouse button click on one of the buttons, if yes then invoke the onclick function
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    sf::Vector2i position = sf::Mouse::getPosition(toolbox.window);
                    sf::Vector2f posFloat(static_cast<float>(position.x), static_cast<float>(position.y));
                    if (toolbox.newGameButton->getSprite()->getGlobalBounds().contains(posFloat)) {
                        toolbox.newGameButton->onClick();
                    } else if (toolbox.debugButton->getSprite()->getGlobalBounds().contains(posFloat)) {
                        toolbox.debugButton->onClick();
                    } else if (toolbox.testButton1->getSprite()->getGlobalBounds().contains(posFloat)) {
                        toolbox.testButton1->onClick();
                    } else if (toolbox.testButton2->getSprite()->getGlobalBounds().contains(posFloat)) {
                        toolbox.testButton2->onClick();
                    } else if (toolbox.testButton3->getSprite()->getGlobalBounds().contains(posFloat)) {
                        toolbox.testButton3->onClick();
                    }
                }
            }
            if (toolbox.gameState->getPlayStatus() == GameState::PLAYING) {     //check if the current state is playing then allow to click on tiles
                faceSprite.setTexture(faceHappyTexture);        //set the sprite for newGameButton to the default one
                toolbox.newGameButton->setSprite(&faceSprite);
                if(event.type == sf::Event::MouseButtonPressed) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {      //check if it is left-clicked
                        sf::Vector2i position = sf::Mouse::getPosition(toolbox.window);     //find the positon of the tile user clicked on if available then run function
                        int x_pos = position.x / 32;
                        int y_pos = position.y / 32;
                        if (toolbox.gameState->getTile(y_pos,x_pos) != nullptr) {
                            toolbox.gameState->getTile(y_pos,x_pos)->onClickLeft();
                        }
                    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {      //check if it is right-clicked
                        sf::Vector2i position = sf::Mouse::getPosition(toolbox.window);     //find the positon of the tile user clicked on if available then run function
                        int x_pos = position.x / 32;
                        int y_pos = position.y / 32;
                        if (toolbox.gameState->getTile(y_pos,x_pos) != nullptr) {
                            toolbox.gameState->getTile(y_pos,x_pos)->onCLickRight();
                        }
                    }
                }
            } else if (toolbox.gameState->getPlayStatus() == GameState::LOSS) {        //check if current state is loss
                faceSprite.setTexture(faceLoseTexture);     //change the face button to lose face
                toolbox.newGameButton->setSprite(&faceSprite);
                for (size_t i = 0; i < 16; ++i) {       //loop through all the tiles to explode the mine tiles
                    for (size_t j = 0; j < 25; ++j) {
                        if (toolbox.gameState->getTile(i, j) != nullptr) {
                            Tile *tile = toolbox.gameState->getTile(i, j);
                            MineTile *ptr = dynamic_cast<MineTile *>(tile);
                            if (ptr != nullptr) {
                                ptr->setState(Tile::HIDDEN);
                                ptr->onClickLeft();
                            }
                        }
                    }
                }
            } else if (toolbox.gameState->getPlayStatus() == GameState::WIN) {      //check if current state is win
                faceSprite.setTexture(faceWinTexture);      //set face button to win face
                toolbox.newGameButton->setSprite(&faceSprite);
            }
            toolbox.window.clear(sf::Color::White);     //clear the window
            render();       //render all the UI elements
            toolbox.window.display();       //display
        }
    }
    //delete all heap pointers
    delete toolbox.gameState;
    delete toolbox.newGameButton;
    delete toolbox.debugButton;
    delete toolbox.testButton1;
    delete toolbox.testButton2;
    delete toolbox.testButton3;
    return 0;
}
void restart() {        //delete the old game state, set debug mode to false and create a new one
    Toolbox& toolbox = Toolbox::getInstance();
    toolbox.debugMode = false;
    delete toolbox.gameState;
    toolbox.gameState = new GameState();
}
void render() {
    Toolbox& toolbox = Toolbox::getInstance();

    //create a vector of sprites that represents different digits based on index
    sf::Texture digitsTexture;
    digitsTexture.loadFromFile("images/digits.png");
    std::vector<sf::Sprite> arr;
    sf::Sprite digit0(digitsTexture, sf::IntRect(0, 0, 21, 32));
    sf::Sprite digit1(digitsTexture, sf::IntRect(21, 0, 21, 32));
    sf::Sprite digit2(digitsTexture, sf::IntRect(42, 0, 21, 32));
    sf::Sprite digit3(digitsTexture, sf::IntRect(63, 0, 21, 32));
    sf::Sprite digit4(digitsTexture, sf::IntRect(84, 0, 21, 32));
    sf::Sprite digit5(digitsTexture, sf::IntRect(105, 0, 21, 32));
    sf::Sprite digit6(digitsTexture, sf::IntRect(126, 0, 21, 32));
    sf::Sprite digit7(digitsTexture, sf::IntRect(147, 0, 21, 32));
    sf::Sprite digit8(digitsTexture, sf::IntRect(168, 0, 21, 32));
    sf::Sprite digit9(digitsTexture, sf::IntRect(189, 0, 21, 32));
    sf::Sprite digitMinus(digitsTexture, sf::IntRect(210, 0, 21, 32));
    arr.push_back(digit0);
    arr.push_back(digit1);
    arr.push_back(digit2);
    arr.push_back(digit3);
    arr.push_back(digit4);
    arr.push_back(digit5);
    arr.push_back(digit6);
    arr.push_back(digit7);
    arr.push_back(digit8);
    arr.push_back(digit9);
    arr.push_back(digitMinus);

    //position on screen for the three box display flag counter
    sf::Vector2f pos1(0.f, 512.f);
    sf::Vector2f pos2(21.f, 512.f);
    sf::Vector2f pos3(42.f, 512.f);

    //calculate the number of flag counter to display
    int remainingFlag = toolbox.gameState->getMineCount() - toolbox.gameState->getFlagCount();

    int first, second, third;
    //determine the digit for the first, second, and third box
    if (remainingFlag >= 0) {
        first = remainingFlag / 100;
        second = (remainingFlag % 100) / 10;
        third = remainingFlag % 10;
    } else {
        first = 10;
        second = (abs(remainingFlag) % 100) / 10;
        third = abs(remainingFlag) % 10;
    }
    for (size_t i = 0; i < arr.size(); ++i) {       //draw the digit with the according sprite
        if (i == first) {
            arr[i].setPosition(pos1);
            toolbox.window.draw(arr[i]);
        }
        if (i == second) {
            arr[i].setPosition(pos2);
            toolbox.window.draw(arr[i]);
        }
        if (i == third) {
            arr[i].setPosition(pos3);
            toolbox.window.draw(arr[i]);
        }
    }

    //draw all the functional button to the screen
    toolbox.window.draw(*toolbox.newGameButton->getSprite());
    toolbox.window.draw(*toolbox.debugButton->getSprite());
    toolbox.window.draw(*toolbox.testButton1->getSprite());
    toolbox.window.draw(*toolbox.testButton2->getSprite());
    toolbox.window.draw(*toolbox.testButton3->getSprite());

    //create sprite for mine image
    sf::Texture mineTexture;
    mineTexture.loadFromFile("images/mine.png");
    sf::Sprite sprite(mineTexture);

    bool winCheck = true;
    for (size_t i = 0; i < 16; ++i) {       //loop through all the tiles
        for (size_t j = 0; j < 25; ++j) {
            if (toolbox.gameState->getTile(i, j) != nullptr) {
                toolbox.gameState->getTile(i, j)->draw();       //draw the current state of the tiles
                Tile* tile = toolbox.gameState->getTile(i, j);
                MineTile* ptr = dynamic_cast<MineTile*>(tile);
                if (getDebugMode()) {       //if debug mode is on, draw mines on all the mine tiles
                    if (ptr != nullptr) {
                        sprite.setPosition(ptr->getLocation());
                        toolbox.window.draw(sprite);
                    }
                }
                if (ptr == nullptr && tile->getState() != Tile::REVEALED) {     //check if win condition is met
                    winCheck = false;
                }
            }
        }
    }
    if (winCheck) {     //if win then set state to WIN
        toolbox.gameState->setPlayStatus(GameState::WIN);
    }
}
void toggleDebugMode() {       //flip the value of toolbox.debugMode
    Toolbox& toolbox = Toolbox::getInstance();
    if (toolbox.debugMode) {
        toolbox.debugMode = false;
    } else {
        toolbox.debugMode = true;
    }
}
bool getDebugMode() {       //return true if toolbox.debugMode is true, false otherwise
    Toolbox& toolbox = Toolbox::getInstance();
    return toolbox.debugMode;
}
int main() {
    return launch();
}

