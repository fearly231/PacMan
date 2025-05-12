#pragma once
#include "Character.h"
class Ghost : public Character {
public:
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
    sf::Vector2f pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);;
};