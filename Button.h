#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button(sf::Vector2f _position, std::function<void(void)> _onClick);     //constructor
    sf::Vector2f getPosition();     //return the position of the button
    sf::Sprite* getSprite();        //return the current sprite of the button
    void setSprite(sf::Sprite* _sprite);        //set the current sprite for the button
    void onClick();     //invoke the function callback method
private:
    sf::Vector2f position;
    sf::Sprite* sprite;
    std::function<void(void)> onClickCallBack;
};