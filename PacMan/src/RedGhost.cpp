#include "RedGhost.h"
#include "YellowGuy.h"
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <random>

RedGhost::RedGhost(int startX, int startY, sf::Texture& normalTexture, sf::Texture& fearedTexture)
    : Ghost(startX, startY, normalTexture), normalTexture(normalTexture), fearedTexture(fearedTexture) {
    gridPos = { startX, startY };
    currentDir = { 0, 0 };
    nextDir = { 0, 0 };
    pixelPos = sf::Vector2f(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2, gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);
   // std::cout << "PixelPos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
    sprite = sf::Sprite(normalTexture);
    //sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameSize) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);
    
}

void RedGhost::update(Board& board, float deltaTime, const sf::Vector2i& pacmanPos) {


    sf::Vector2f targetPixel(gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
        gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2);

    if (std::abs(pixelPos.x - targetPixel.x) < 2 && std::abs(pixelPos.y - targetPixel.y) < 2) {
        pixelPos = targetPixel;

        auto path = getPathTo(board, gridPos, pacmanPos);
        if (!path.empty()) {
            sf::Vector2i nextStep = path[0];
            currentDir = nextStep - gridPos;
            gridPos = nextStep;
        }
        if (pacmanPos == gridPos and isFeared(board) == false) {
            std::cout << "KONIEC GRY, PRZEGRALES";
           
        }
        
    }
    if (pacmanPos == gridPos && isFeared(board)) {
       // std::cout << "Zjad³eœ RedGhosta\n";
        //counter = counter + 100;
		eated.openFromFile("assets/music/pacman_eatghost.wav");
        eated.play();
        gridPos.x = Board::WIDTH / 2;
        gridPos.y = Board::HEIGHT / 2;

        pixelPos = sf::Vector2f(
            gridPos.x * Board::TILE_SIZE + Board::TILE_SIZE / 2,
            gridPos.y * Board::TILE_SIZE + Board::TILE_SIZE / 2
        );
        comingBack.openFromFile("assets/music/eatedghost.ogg");
        comingBack.setVolume(30);
        comingBack.play();
        sprite.setPosition(pixelPos);
        currentDir = { 0, 0 };
    }
    sf::Vector2f movement(currentDir.x * speed * deltaTime, currentDir.y * speed * deltaTime);
    pixelPos += movement;
    sprite.setPosition(pixelPos);
    if (gridPos.x <= 0) {
        gridPos.x = Board::WIDTH - 2; // Przeniesienie na praw¹ stronê mapy
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
    else if (gridPos.x >= Board::WIDTH - 1) {
        gridPos.x = 1; // Przeniesienie na lew¹ stronê mapy
        pixelPos.x = gridPos.x * Board::TILE_SIZE + 2 + Board::TILE_SIZE / 2;
    }
}


std::vector<sf::Vector2i> RedGhost::getPathTo(Board& board, sf::Vector2i start, sf::Vector2i goal) {
    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    // Tryb FEARED
    if (isFeared(board)) {
        speed = 50.f;
        sprite = sf::Sprite(fearedTexture);
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
        sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
        sprite.setScale(sf::Vector2f(1.5f, 1.5f));
        sprite.setPosition(pixelPos);

        std::vector<sf::Vector2i> options;
        for (const auto& dir : directions) {
            sf::Vector2i next = start + dir;
            if (!board.isWall(next.x, next.y)) {
                options.push_back(next);
            }
        }

        if (options.empty()) return {};
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, options.size() - 1);
        sf::Vector2i randomDirection = options[dis(gen)];

        return { randomDirection };
    }

    // Tryb CHASE
    speed = 80.f;
    sprite = sf::Sprite(normalTexture);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameSize, frameHeight }));
    sprite.setOrigin(sf::Vector2f(static_cast<float>(frameSize) / 2.f, static_cast<float>(frameHeight) / 2.f));
    sprite.setScale(sf::Vector2f(1.5f, 1.5f));
    sprite.setPosition(pixelPos);

    // Jeœli goal to œciana, przesuñ go w stronê startu
    if (board.isWall(goal.x, goal.y)) {
        for (const auto& dir : directions) {
            sf::Vector2i adjusted = goal + dir;
            if (!board.isWall(adjusted.x, adjusted.y)) {
                goal = adjusted;
                break;
            }
        }
    }

    std::queue<std::pair<sf::Vector2i, int>> frontier;
    std::map<sf::Vector2i, sf::Vector2i, Vec2iLess> cameFrom;

    frontier.push({ start, 0 });
    cameFrom[start] = start;

    const int maxDepth = 50;

    int bfsSteps = 0;
    const int maxSteps = 1000;
    while (!frontier.empty() && bfsSteps++ < maxSteps) {
        auto [current, depth] = frontier.front();
        frontier.pop();

        if (depth > maxDepth) break;
        if (current == goal) break;

        for (const auto& dir : directions) {
            sf::Vector2i next = current + dir;

            if (!board.isWall(next.x, next.y) && cameFrom.find(next) == cameFrom.end()) {
                frontier.push({ next, depth + 1 });
                cameFrom[next] = current;
            }
        }
    }

    std::vector<sf::Vector2i> path;
    if (cameFrom.find(goal) == cameFrom.end()) return path;

    for (sf::Vector2i current = goal; current != start; current = cameFrom[current]) {
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}
void RedGhost::draw(sf::RenderWindow& window) {
    //window.draw(sprite);
    sprite.setPosition(pixelPos);
    //std::cout << "RedGhost position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
   
    window.draw(sprite);
    
    
}
bool RedGhost::isFeared(Board& board) {
    return board.isCherryActive();
}
sf::Vector2i RedGhost::getRedGhostPos(){
    return gridPos;
}
