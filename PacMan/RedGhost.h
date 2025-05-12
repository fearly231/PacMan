#pragma once
#include "Ghost.h"



struct Vec2iLess {
	bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
		if (lhs.y != rhs.y) return lhs.y < rhs.y;
		return lhs.x < rhs.x;
	}
};
class RedGhost : public Ghost {
public:

	RedGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture);
	void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) override;
	std::vector<sf::Vector2i> getPathTo(Board& board, sf::Vector2i start, sf::Vector2i goal);
	//void setTarget(sf::Vector2i target);
	void draw(sf::RenderWindow& window) override;
	bool isFeared(Board& board) override;
	
private:
	sf::Texture& normalTexture;
	sf::Texture& fearedTexture;
};


