#pragma once
#include "Ghost.h"
#include <SFML/Audio.hpp>
class OrangeGhost : public Ghost {
public:

	OrangeGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture);
	void update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos, const sf::Vector2i& pacmanDir);
	std::vector<sf::Vector2i> getPathTo(Board& board, sf::Vector2i start, sf::Vector2i pacmanPos);
	void draw(sf::RenderWindow& window) override;
	bool isFeared(Board& board) override;
	//void setRedGhostReference(RedGhost& red);
	~OrangeGhost() {};

private:
	sf::Texture& normalTexture;
	sf::Texture& fearedTexture;
	sf::Music eated;
	sf::Music comingBack;
};

