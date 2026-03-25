#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>

const int CELL = 20;
const int COLS = 30;
const int ROWS = 25;
const int W = COLS * CELL;
const int H = ROWS * CELL;

struct Vec { int x, y; };

Vec randomFood(const std::deque<Vec>& snake) {
    Vec f;
    do {
        f = { rand() % COLS, rand() % ROWS };
        bool hit = false;
        for (auto& s : snake) if (s.x == f.x && s.y == f.y) { hit = true; break; }
        if (!hit) break;
    } while (true);
    return f;
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode({ (unsigned)W, (unsigned)H }), "Snake");
    window.setFramerateLimit(10);

    std::deque<Vec> snake = { {COLS/2, ROWS/2} };
    Vec dir = {1, 0};
    Vec nextDir = dir;
    Vec food = randomFood(snake);
    int score = 0;
    bool gameOver = false;

    sf::Font font;
    bool hasFont = font.openFromFile("C:/Windows/Fonts/arial.ttf");

    sf::Text scoreText(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 5.f, 5.f });

    sf::Text overText(font);
    overText.setCharacterSize(36);
    overText.setFillColor(sf::Color::Red);
    overText.setString("GAME OVER\nPress R to restart");
    overText.setPosition({ W/2.f - 130.f, H/2.f - 40.f });

    sf::RectangleShape cell({ (float)CELL - 2, (float)CELL - 2 });

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* k = event->getIf<sf::Event::KeyPressed>()) {
                if (!gameOver) {
                    if (k->code == sf::Keyboard::Key::Up    && dir.y == 0) nextDir = {0,-1};
                    if (k->code == sf::Keyboard::Key::Down  && dir.y == 0) nextDir = {0, 1};
                    if (k->code == sf::Keyboard::Key::Left  && dir.x == 0) nextDir = {-1,0};
                    if (k->code == sf::Keyboard::Key::Right && dir.x == 0) nextDir = { 1,0};
                }
                if (gameOver && k->code == sf::Keyboard::Key::R) {
                    snake = { {COLS/2, ROWS/2} };
                    dir = {1, 0}; nextDir = dir;
                    food = randomFood(snake);
                    score = 0; gameOver = false;
                }
            }
        }

        if (!gameOver) {
            dir = nextDir;
            Vec head = { snake.front().x + dir.x, snake.front().y + dir.y };

            // Wall collision
            if (head.x < 0 || head.x >= COLS || head.y < 0 || head.y >= ROWS)
                gameOver = true;

            // Self collision
            for (auto& s : snake)
                if (s.x == head.x && s.y == head.y) { gameOver = true; break; }

            if (!gameOver) {
                snake.push_front(head);
                if (head.x == food.x && head.y == food.y) {
                    score++;
                    food = randomFood(snake);
                } else {
                    snake.pop_back();
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // Draw food
        cell.setFillColor(sf::Color(220, 50, 50));
        cell.setPosition({ (float)food.x * CELL + 1, (float)food.y * CELL + 1 });
        window.draw(cell);

        // Draw snake
        for (int i = 0; i < (int)snake.size(); i++) {
            cell.setFillColor(i == 0 ? sf::Color(100, 220, 100) : sf::Color(50, 180, 50));
            cell.setPosition({ (float)snake[i].x * CELL + 1, (float)snake[i].y * CELL + 1 });
            window.draw(cell);
        }

        // Draw score
        if (hasFont) {
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            if (gameOver) window.draw(overText);
        }

        window.display();
    }
    return 0;
}
