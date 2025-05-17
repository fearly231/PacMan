#include <SFML/Graphics.hpp>
#include "Board.h"
#include "YellowGuy.h"
#include "Character.h"
#include "RedGhost.h"
#include "BlueGhost.h"
#include "OrangeGhost.h"
#include "PinkGhost.h"
#include <iostream>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>

int main() {
    std::string nick;
    std::cout << "Wpisz swoj nick: ";
    std::getline(std::cin, nick);

    bool gameEnded = false;

    sf::RenderWindow window(sf::VideoMode({ Board::WIDTH * Board::TILE_SIZE, Board::HEIGHT * Board::TILE_SIZE }), "Pac-Man");
    Board board;
    int totalPoint = 12;// board.countTotalPoints();

    sf::Clock clock;
    sf::Clock clock2;

    sf::Texture pacManTexture, redGhostTexture, blueGhostTexture, orangeGhostTexture, pinkGhostTexture, fearedTexture;
    if (!pacManTexture.loadFromFile("assets/images/PacMan32.png") ||
        !redGhostTexture.loadFromFile("assets/images/redghost.png") ||
        !fearedTexture.loadFromFile("assets/images/feared.png") ||
        !blueGhostTexture.loadFromFile("assets/images/blueghost.png") ||
        !orangeGhostTexture.loadFromFile("assets/images/orangeghost.png") ||
        !pinkGhostTexture.loadFromFile("assets/images/pinkghost.png")) {
        std::cerr << "Could not load textures!" << std::endl;
        return -1;
    }

    YellowGuy pacman(12, 5, pacManTexture);
    RedGhost redGhost(13, 13, redGhostTexture, fearedTexture);
    BlueGhost blueGhost(12, 12, blueGhostTexture, fearedTexture);
    OrangeGhost orangeGhost(11, 11, orangeGhostTexture, fearedTexture);
    PinkGhost pinkGhost(13, 12, pinkGhostTexture, fearedTexture);

    sf::Vector2i pacmanDir = pacman.getDirPosition();
    sf::Vector2i blinkyPos = redGhost.getRedGhostPos();
    clock2.restart();
    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
		float time = clock2.getElapsedTime().asSeconds();
		std::cout << "Elapsed time: " << time << " seconds" << std::endl;
        pacman.handleInput();
        pacman.update(board, deltaTime);
        redGhost.update(board, deltaTime, pacman.getGridPosition());
        blueGhost.update(board, deltaTime, pacman.getGridPosition(), pacmanDir, blinkyPos);
        orangeGhost.update(board, deltaTime, pacman.getGridPosition(), pacmanDir);
        pinkGhost.update(board, deltaTime, pacman.getGridPosition(), pacmanDir);
        board.updateCherry(deltaTime);

        if (pacman.counter == totalPoint && !gameEnded) {
            gameEnded = true;
            window.close();
            int score = static_cast<int>(pacman.counter / time * 200);
            //float timeElapsed = clock.getElapsedTime().asSeconds();

            // Zapis do pliku
            try {
                std::ofstream file("score.txt", std::ios::app);
                if (file.is_open()) {
                    file << "Nick: " << nick
                        << " | Score: " << score
                        << " | Time: " << std::fixed << std::setprecision(2) << time << "s\n";
                    
                    file.close();
					
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Błąd przy zapisie wyniku: " << e.what() << std::endl;
            }

            // Wyświetlenie ekranu zwycięstwa
            sf::RenderWindow winnerScreen(sf::VideoMode({ 800, 600 }), "You Win!", sf::Style::Close | sf::Style::Titlebar);
            sf::Font font("assets/fonts/arial.ttf");

         
            sf::Text title(font);
            title.setString("Zwyciestwo!");
            title.setCharacterSize(60);
            title.setFillColor(sf::Color::Yellow);
            title.setStyle(sf::Text::Bold);
            title.setPosition(sf::Vector2f(800.f / 2.f - 180.f, 80.f));

            sf::Text scoreText(font);
            scoreText.setString("Twój wynik to: " + std::to_string(score));
            scoreText.setCharacterSize(40);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(sf::Vector2f(800.f / 2.f - 200.f, 210.f));



            sf::Text nickText(font);
            nickText.setString("Gracz: " + nick);
            nickText.setCharacterSize(40);
            nickText.setFillColor(sf::Color::Cyan);
            nickText.setPosition({ 800.f / 2.f - 200.f, 250.f });

            sf::Text choise(font);
            choise.setCharacterSize(20);
            choise.setString("Nacisnij ESC aby zakonczyc gre lub Enter aby zaczac od nowa");
            choise.setPosition({10,10});
            choise.setFillColor(sf::Color::Green);

            sf::Text choise1(font);
            choise1.setString("Wyswietl wyniki: nacisnij W");
            choise1.setFillColor(sf::Color::Magenta);
            choise1.setPosition({ 10.f, 550.f });

            std::ifstream file("assets/data/score.txt");

            while (winnerScreen.isOpen()) {
                while (std::optional<sf::Event> event = winnerScreen.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            winnerScreen.close();
                        }


                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                            winnerScreen.close();
                            main();

                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {

                            if (file.is_open()) {
                                std::string line;
                                while (std::getline(file, line)) {
                                    std::cout << line << std::endl;
                                }
                                file.close();
                            }
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                            winnerScreen.close();
     
                        }

                    }

                    winnerScreen.clear();
                    winnerScreen.draw(title);
                    winnerScreen.draw(scoreText);
                    winnerScreen.draw(nickText);
                    winnerScreen.draw(choise);
                    winnerScreen.draw(choise1);
                    winnerScreen.display();
                }

            }
            window.clear();
            board.draw(window);
            redGhost.draw(window);
            pinkGhost.draw(window);
            blueGhost.draw(window);
            orangeGhost.draw(window);
            pacman.draw(window);
            window.display();
        }

    }

