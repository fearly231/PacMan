#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Character.h"
class YellowGuy : public Character {
public:
    YellowGuy(int startX, int startY, sf::Texture& texture);
    void handleInput();                // Odczyt klawiatury
    void update(Board& board, float deltaTime);   // Ruch z uwzglêdnieniem œcian
    void draw(sf::RenderWindow& window) override;
    void collectPoint(Board& board);
    int getCounter() {
        return counter;
    }
    int counter = 0;
private:
   
    sf::Vector2f pixelPos;
    sf::CircleShape shape;
    float speed = 100.0f; // piksele na sekundê
    bool canMove(const Board& board, sf::Vector2i dir) const;
    void updateAnimation(float deltaTime);
   


    const int frameCount = 3;
    const int frameSize = 32;
    float animationTimer = 0.0f;
    float animationDelay = 0.1f;
    int currentFrame = 0;

};
