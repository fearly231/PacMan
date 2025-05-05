#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
Board::Board() {
    level = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
    {2,2,2,2,2,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,2,2,2,2,2},
    {2,2,2,2,2,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,2,2,2,2,2,},
    {2,2,2,2,2,1,0,1,1,0,1,1,0,0,1,1,0,1,1,0,1,2,2,2,2,2},
    {1,1,1,1,1,1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1,1,1,1,1,1,},
    {0,0,0,0,0,0,0,0,0,0,1,2,4,2,2,1,0,0,0,0,0,0,0,0,0,0,},
    {1,1,1,1,1,1,0,1,1,0,1,2,2,2,2,1,0,1,1,0,1,1,1,1,1,1,},
    {2,2,2,2,2,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,2,2,2,2,2},
    {2,2,2,2,2,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,2,2,2,2,2,},
    {2,2,2,2,2,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,2,2,2,2,2},
    {1,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    };
	wallShape.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	wallShape.setFillColor(sf::Color::Black);
	wallShape.setOutlineThickness(1);
	wallShape.setOutlineColor(sf::Color::Blue);
};


Board::~Board() {};



void Board::draw(sf::RenderWindow& window) {
    sf::CircleShape dotShape(TILE_SIZE / 6);
    dotShape.setFillColor(sf::Color::White);

    sf::RectangleShape box(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    box.setFillColor(sf::Color::Transparent);

    sf::RectangleShape ghostShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    ghostShape.setFillColor(sf::Color::Red);


    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (level[y][x] == 1) {
                wallShape.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                window.draw(wallShape);
            }
            else if (level[y][x] == 0) {
                dotShape.setPosition(sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 3, y * TILE_SIZE + TILE_SIZE / 3));
                window.draw(dotShape);
            }
            else if (level[y][x] == 2) {
                box.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                window.draw(box);
            }
            else if (level[y][x] == 3) {
                ghostShape.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
                window.draw(ghostShape);
            }
            else if (level[y][x] == 4) {
                ghostShape.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
            }
        }

    }
}



bool Board::isWall(int x, int y) const {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return false;
	}
    std::cout << "sciana" << std::endl;
	return level[y][x] == 1;
	
}
bool Board::isPoint(int x, int y) {
    if (level[y][x] == 0){
		level[y][x] = 2; // zmiana na 2
        return true;

	}
    return false;
}
int Board::countTotalPoints() {
    int count = 0;
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (level[y][x] == 0) count++;
        }
    }
    std::cout << count;
    return count;
}