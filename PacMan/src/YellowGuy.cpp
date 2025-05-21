#include "YellowGuy.h"
#include "Board.h"
#include "Character.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

YellowGuy::YellowGuy(int startX, int startY, sf::Texture& texture) : Character(startX, startY, speed)
{
    gridPos = { startX, startY };
    currentDir = { 0, 0 };
    nextDir = { 0, 0 };
    pixelPos = sf::Vector2f( gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2 );
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({ 32, 0 }, {frameSize, frameSize}));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameSize) /2.f));
    sprite.setPosition(pixelPos);
	sprite.setScale(sf::Vector2f(0.7f, 0.7f));
   // std::cout << "PixelPos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
    
}

void YellowGuy::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        //std::cout << "Up key pressed!" << std::endl;
        nextDir = { 0, -1 };
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
       // std::cout << "Down key pressed!" << std::endl;
        nextDir = { 0, 1 };
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
       // std::cout << "Left key pressed!" << std::endl; 
        nextDir = { -1, 0 };
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
       // std::cout << "Right key pressed!" << std::endl;
        nextDir = { 1, 0 };
    }
}

bool YellowGuy::canMove(const Board& board, sf::Vector2i dir) const {
    sf::Vector2i next = gridPos + dir;
    return !board.isWall(next.x, next.y);
    


}

void YellowGuy::collectPoint(Board& board)  {
	
		if (board.isPoint(gridPos.x, gridPos.y) == true) {
			counter++;
   
            music.openFromFile("assets/music/PacManWakaWaka.ogg");
			music.setVolume(20);
            music.play();
		
            
            
			//std::cout << counter << std::endl;
           
		}
      
        if (board.isCherry(gridPos.x, gridPos.y) == true) {
            board.activateCherry(7.f); // 7 sekund strachu
            scared.openFromFile("assets/music/scared.ogg");
            scared.play();
            
        }
        
};

void YellowGuy::update(Board& board, float deltaTime) {
    //std::cout << "Updating..." << std::endl;
   
	
    sf::Vector2f targetPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);

	//std::cout << "TargetPos: " << targetPos.x << ", " << targetPos.y << std::endl;
    if (gridPos.x <= 0) {
        gridPos.x = Board::WIDTH - 2; // Przeniesienie na praw¹ stronê mapy
        pixelPos.x = gridPos.x * Board::TILE_SIZE+ 1 + Board::TILE_SIZE / 2;
    }
    else if (gridPos.x >= Board::WIDTH - 1) {
        gridPos.x = 1; // Przeniesienie na lew¹ stronê mapy
        pixelPos.x = gridPos.x * Board::TILE_SIZE+ 1 + Board::TILE_SIZE / 2;
    }

    else if (std::abs(pixelPos.x - targetPos.x) < 2 && std::abs(pixelPos.y - targetPos.y) < 2) {
        pixelPos = targetPos; // snap to grid
        if (canMove(board, nextDir)) {
            currentDir = nextDir;
			//std::cout << "CurrentDir: " << currentDir.x << ", " << currentDir.y << std::endl;
            // Ustawianie rotacji po wejœciu na now¹ œcie¿kê
           
            if (currentDir == sf::Vector2i(0, -1)) {
                sprite.setRotation(sf::degrees(270));
            }
            else if (currentDir == sf::Vector2i(0, 1)) {
                sprite.setRotation(sf::degrees(90));
            }
            else if (currentDir == sf::Vector2i(-1, 0)) {
                sprite.setRotation(sf::degrees(180));
            }
            else if (currentDir == sf::Vector2i(1, 0)) {
                sprite.setRotation(sf::degrees(0));
            }
 
        }

        if (canMove(board, currentDir)) {
            gridPos += currentDir;
			//std::cout << "GridPos: " << gridPos.x << ", " << gridPos.y << std::endl;
           
        }
        else if (!canMove(board, currentDir)) {
            currentDir = { 0, 0 };
        }
    }

    // Przesuwaj w stronê nowej kratki
    sf::Vector2f movement(currentDir.x * speed * deltaTime, currentDir.y * speed * deltaTime);
    pixelPos += movement;
    sprite.setPosition(pixelPos);
    collectPoint(board);
   // board.isCherry(board)
    updateAnimation(deltaTime);
}

void YellowGuy::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationDelay) {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;
        sprite.setTextureRect(sf::IntRect({ currentFrame * frameSize, 0 }, {frameSize, frameSize}));
    }
}

void YellowGuy::draw(sf::RenderWindow& window) {
    //window.draw(sprite);
    sprite.setPosition(pixelPos);
    //std::cout << "position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
    window.draw(sprite);
}
sf::Vector2i YellowGuy::getDirPosition() {
    return currentDir;
}
