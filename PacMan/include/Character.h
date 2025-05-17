#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>
class Character
{
    public:
        Character(int startX, int startY, float speed);
        virtual void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos);
        //virtual void update(Board& board, float deltaTime) = 0; // Czysta funkcja wirtualna
        virtual void draw(sf::RenderWindow& window); // Czysta funkcja wirtualna
        virtual ~Character(); // Wirtualny destruktor
        sf::Vector2i getGridPosition();
    protected:
        sf::Vector2i gridPos;
        sf::Vector2f pixelPos;
        sf::Vector2i currentDir;
        sf::Vector2i nextDir;
        sf::Sprite sprite;
        float speed;

};

