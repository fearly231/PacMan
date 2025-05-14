#pragma once
#include "Ghost.h"




class RedGhost : public Ghost {
public:

	RedGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture);
	void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) override;
	std::vector<sf::Vector2i> getPathTo(Board& board, sf::Vector2i start, sf::Vector2i goal);
	//void setTarget(sf::Vector2i target);
	void draw(sf::RenderWindow& window) override;
	bool isFeared(Board& board) override;
	sf::Vector2i getRedGhostPos();
	
private:
	sf::Texture& normalTexture;
	sf::Texture& fearedTexture;
	
};


