#include <SFML/Graphics.hpp>
#include "Board.h"
#include "YellowGuy.h"
#include "Character.h"
#include "RedGhost.h"
#include <iostream>
#include <exception>

int main() {


  
   

    // Create the window with the specified settings

    sf::RenderWindow window(sf::VideoMode({ Board::WIDTH * Board::TILE_SIZE, Board::HEIGHT * Board::TILE_SIZE }), "Pac-Man");
    Board board;
	int totalPoint = board.countTotalPoints();
    sf::Clock clock;
   
    sf::Texture pacManTexture;
    if (!pacManTexture.loadFromFile("PacMan32.png"))
    {
        std::cerr << "could not load texture!"; // Handle error
        return -1;
    }
    sf::Texture redGhostTexture;
    if (!redGhostTexture.loadFromFile("redghost.png"))
    {
        std::cerr << "could not load texture!"; // Handle error
        return -1;
    }
    sf::Texture fearedTexture;
    if (!fearedTexture.loadFromFile("feared.png"))
    {
        std::cerr << "could not load texture!"; // Handle error
        return -1;
    }
   YellowGuy pacman(1, 1, pacManTexture);
   RedGhost redGhost(13, 13, redGhostTexture, fearedTexture);

   
    while (window.isOpen())


    {
  
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        float deltaTime = clock.restart().asSeconds();
        pacman.handleInput();
        pacman.update(board, deltaTime);
        redGhost.update(board, deltaTime, pacman.getGridPosition());
        board.updateCherry(deltaTime);

        if (pacman.counter == totalPoint)
		{
			std::cout << "You win!" << std::endl;
			window.close();
            sf::RenderWindow winnerScreen(sf::VideoMode({ 800, 600 }), "You Win!", sf::Style::Close | sf::Style::Titlebar);
            while (winnerScreen.isOpen()) {
                int score = pacman.counter / deltaTime;
                sf::Font font("arial.ttf");
                sf::Text message(font);
                message.setString("You win!\n Your score is: "  + std::to_string(score) );
                message.setCharacterSize(50);
                message.setFillColor(sf::Color::Red);
                message.setPosition({ 100.f, 200.f });
           
                
				winnerScreen.clear();
				winnerScreen.draw(message);
				
				winnerScreen.display();
				while (std::optional<sf::Event> event = winnerScreen.pollEvent()) {
					if (event->is<sf::Event::Closed>()) {
						winnerScreen.close();
					}
				}


            }
            
		}
        window.clear();
        board.draw(window);
        redGhost.draw(window);
        pacman.draw(window);
        window.display();

	
    }
}