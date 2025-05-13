#include "Ghost.h"
Ghost::Ghost(int startX, int startY, sf::Texture& texture)
    : Character(startX, startY, 80.f) {
    sprite.setTexture(texture);
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameSize) / 2.f));
    sprite.setPosition(pixelPos);

}
void Ghost::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) {
    sprite.setPosition(pixelPos);

    if (feared && fearTimer.getElapsedTime().asSeconds() > 5.f) {
        feared = false;
        
    }

 }
void Ghost::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pixelPos);
    window.draw(sprite);
}
bool Ghost::isFeared(Board& board) {
    return board.isCherry(gridPos.x, gridPos.y);
}




