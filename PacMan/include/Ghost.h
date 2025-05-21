#pragma once
#include "Character.h"
#include <SFML/Audio.hpp>
class Ghost : public Character {
public:
    struct Vec2iLess {
        bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
            if (lhs.y != rhs.y) return lhs.y < rhs.y;
            return lhs.x < rhs.x;
        }
    };
    Ghost(int startX, int startY, sf::Texture& texture);
    virtual void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) override;
    virtual void draw(sf::RenderWindow& window) override;
    virtual bool isFeared(Board& board);
    virtual ~Ghost() {}
    

protected:
    const int frameCount = 4;
    const int frameSize = 14;
    const int frameHeight = 16;
    bool feared = false;
    sf::Clock fearTimer;
    //sf::Sprite sprite;
    sf::Vector2f pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
};