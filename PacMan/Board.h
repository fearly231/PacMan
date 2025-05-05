#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Board {
public:
    static const int TILE_SIZE = 20;
    static const int WIDTH = 26;
    static const int HEIGHT = 29;
    Board();
    ~Board();
    void draw(sf::RenderWindow& window);
    bool isWall(int x, int y) const;
	bool isPoint(int x, int y);
    int countTotalPoints();

private:
    std::vector<std::vector<int>> level;
    sf::RectangleShape wallShape;

};