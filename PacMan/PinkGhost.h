#pragma once
#include "Ghost.h"

class PinkGhost : public Ghost
{
public:


	PinkGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture);
	void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos, sf::Vector2i& pacmanDir);
	std::vector<sf::Vector2i> getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos, sf::Vector2i pacmanDir);
	void draw(sf::RenderWindow& window) override;
	bool isFeared(Board& board) override;
	//void setRedGhostReference(RedGhost& red);
	~PinkGhost() {};

private:
	sf::Texture& normalTexture;
	sf::Texture& fearedTexture;

};

