#include "Character.h"
#include <SFML/Graphics.hpp>


Character::Character(int startX, int startY, float speed) : gridPos(startX, startY), currentDir(0, 0), nextDir(0, 0), pixelPos(0.f, 0.f), speed(speed), sprite(sprite) {}
Character::~Character() {};
void Character::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) {};
void Character::draw(sf::RenderWindow& window) {
	sprite.setPosition(pixelPos);
	window.draw(sprite);
}
//void Character::draw(sf::RenderWindow& window) {}
sf::Vector2i Character::getGridPosition() {
	return gridPos;
}