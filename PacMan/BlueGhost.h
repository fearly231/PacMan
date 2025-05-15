#pragma once
#include "Ghost.h"
#include "RedGhost.h"
class BlueGhost : public Ghost
{
public: 


	BlueGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture);
	void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos, const sf::Vector2i& pacmanDir, const sf::Vector2i& blinkyPos);
	std::vector<sf::Vector2i> getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos, sf::Vector2i pacmanDir, sf::Vector2i blinkyPos);
	void draw(sf::RenderWindow& window) override;
	bool isFeared(Board& board) override;
	//void setRedGhostReference(RedGhost& red);
	~BlueGhost() {};

private:
	sf::Texture& normalTexture;
	sf::Texture& fearedTexture;

};

