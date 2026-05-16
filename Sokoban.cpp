// Copyright 2026 Aanya Bharti
#include "Sokoban.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace SB {

static std::size_t idx(unsigned x, unsigned y, unsigned w) {
    return x + y * w;
}

Sokoban::Sokoban()
    : moveText(font),
      winText(font),
      winSound(winBuffer) {
    if (!tiles.loadFromFile("sokoban_tilesheet.png"))
        throw std::runtime_error("Failed to load tilesheet");

    if (!font.openFromFile("Roboto-Regular.ttf"))
        throw std::runtime_error("Failed to load font");

    if (!winBuffer.loadFromFile("win.wav"))
        throw std::runtime_error("Failed to load win.wav");

    moveText.setCharacterSize(20);
    moveText.setFillColor(sf::Color::White);
    moveText.setPosition(sf::Vector2f(10.f, 10.f));

    winText.setCharacterSize(40);
    winText.setFillColor(sf::Color::Yellow);
    winText.setString("You Win!");
    winText.setPosition(sf::Vector2f(100.f, 100.f));
}

Sokoban::Sokoban(const std::string& filename)
    : Sokoban() {
    std::ifstream fin(filename);
    if (!fin)
        throw std::runtime_error("Could not open level file");
    fin >> *this;
}

unsigned int Sokoban::height() const { return h; }
unsigned int Sokoban::width() const { return w; }

sf::Vector2u Sokoban::size() const { return {w, h}; }

sf::Vector2u Sokoban::windowSize() const {
    return {w * TILE_SIZE, h * TILE_SIZE};
}

sf::Vector2u Sokoban::playerLoc() const { return player; }

bool Sokoban::isWon() const {
    int boxesTotal = std::count_if(
        grid.begin(), grid.end(),
        [](char c) { return c == 'A' || c == '1'; });

    int storageTotal = std::count_if(
        grid.begin(), grid.end(),
        [](char c) { return c == 'a' || c == '1'; });

    int boxesOnStorage = std::count_if(
        grid.begin(), grid.end(),
        [](char c) { return c == '1'; });

    return boxesOnStorage == std::min(boxesTotal, storageTotal);
}

void Sokoban::movePlayer(Direction dir) {
    if (won)
        return;

    facing = dir;

    int dx = 0;
    int dy = 0;

    if (dir == Direction::Up)
        dy = -1;
    if (dir == Direction::Down)
        dy = 1;
    if (dir == Direction::Left)
        dx = -1;
    if (dir == Direction::Right)
        dx = 1;

    int nx = static_cast<int>(player.x) + dx;
    int ny = static_cast<int>(player.y) + dy;

    if (nx < 0 || ny < 0 ||
        nx >= static_cast<int>(w) ||
        ny >= static_cast<int>(h))
        return;

    char& nextTile = grid[idx(nx, ny, w)];

    if (nextTile == '#')
        return;

    undoGrid.push(grid);
    undoPlayer.push(player);

    if (nextTile == 'A' || nextTile == '1') {
        int bx = nx + dx;
        int by = ny + dy;

        if (bx < 0 || by < 0 ||
            bx >= static_cast<int>(w) ||
            by >= static_cast<int>(h))
            return;

        char& beyondTile = grid[idx(bx, by, w)];

        if (beyondTile == '#' ||
            beyondTile == 'A' ||
            beyondTile == '1')
            return;

        if (beyondTile == 'a')
            beyondTile = '1';
        else
            beyondTile = 'A';

        if (nextTile == '1')
            nextTile = 'a';
        else
            nextTile = '.';
    }

    player = {static_cast<unsigned>(nx),
              static_cast<unsigned>(ny)};

    moves++;

    won = isWon();

    if (won)
        winSound.play();

    if (!won &&
        grid[idx(player.x, player.y, w)] == 'T' &&
        teleporters.size() == 2) {
        if (player == teleporters[0])
            player = teleporters[1];
        else
            player = teleporters[0];
    }
}

void Sokoban::undo() {
    if (undoGrid.empty())
        return;

    redoGrid.push(grid);
    redoPlayer.push(player);

    grid = undoGrid.top();
    player = undoPlayer.top();

    undoGrid.pop();
    undoPlayer.pop();

    won = isWon();
}

void Sokoban::redo() {
    if (redoGrid.empty())
        return;

    undoGrid.push(grid);
    undoPlayer.push(player);

    grid = redoGrid.top();
    player = redoPlayer.top();

    redoGrid.pop();
    redoPlayer.pop();

    won = isWon();
}

void Sokoban::reset() {
    grid = initialGrid;
    player = initialPlayer;
    moves = 0;
    won = false;

    while (!undoGrid.empty())
        undoGrid.pop();

    while (!redoGrid.empty())
        redoGrid.pop();
}

void Sokoban::draw(sf::RenderTarget& target,
                   sf::RenderStates states) const {
    sf::Sprite sprite(tiles);

    for (unsigned y = 0; y < h; y++) {
        for (unsigned x = 0; x < w; x++) {
            sprite.setPosition(
                sf::Vector2f(
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE)));

            char c = grid[idx(x, y, w)];

            if (c == '#') {
                sprite.setTextureRect(
                    sf::IntRect(
                        sf::Vector2i(8 * TILE_SIZE,
                                     6 * TILE_SIZE),
                        sf::Vector2i(TILE_SIZE,
                                     TILE_SIZE)));
                target.draw(sprite);
                continue;
            }

            sprite.setTextureRect(
                sf::IntRect(
                    sf::Vector2i(12 * TILE_SIZE,
                                 6 * TILE_SIZE),
                    sf::Vector2i(TILE_SIZE,
                                 TILE_SIZE)));
            target.draw(sprite);

            if (c == 'T') {
                sprite.setTextureRect(
                    sf::IntRect(
                        sf::Vector2i(10 * TILE_SIZE,
                                     5 * TILE_SIZE),
                        sf::Vector2i(TILE_SIZE,
                                     TILE_SIZE)));
                target.draw(sprite);
            }

            if (c == 'a' || c == '1') {
                sprite.setTextureRect(
                    sf::IntRect(
                        sf::Vector2i(12 * TILE_SIZE,
                                     7 * TILE_SIZE),
                        sf::Vector2i(TILE_SIZE,
                                     TILE_SIZE)));
                target.draw(sprite);
            }

            if (c == 'A' || c == '1') {
                sprite.setTextureRect(
                    sf::IntRect(
                        sf::Vector2i(7 * TILE_SIZE,
                                     0),
                        sf::Vector2i(TILE_SIZE,
                                     TILE_SIZE)));
                target.draw(sprite);
            }
        }
    }

    sprite.setPosition(
        sf::Vector2f(
            static_cast<float>(player.x * TILE_SIZE),
            static_cast<float>(player.y * TILE_SIZE)));

    int px = 1;
    int py = 4;

    if (facing == Direction::Up) {
        px = 4;
        py = 4;
    }
    if (facing == Direction::Right) {
        px = 1;
        py = 6;
    }
    if (facing == Direction::Left) {
        px = 4;
        py = 6;
    }

    sprite.setTextureRect(
        sf::IntRect(
            sf::Vector2i(px * TILE_SIZE,
                         py * TILE_SIZE),
            sf::Vector2i(TILE_SIZE,
                         TILE_SIZE)));

    target.draw(sprite);

    moveText.setString("Moves: " +
                       std::to_string(moves));
    target.draw(moveText);

    if (won)
        target.draw(winText);
}

std::istream& operator>>(std::istream& in,
                         Sokoban& s) {
    in >> s.h >> s.w;

    s.grid.assign(s.w * s.h, '.');
    s.teleporters.clear();

    std::string line;

    for (unsigned y = 0; y < s.h; y++) {
        in >> line;

        for (unsigned x = 0; x < s.w; x++) {
            char c = line[x];

            if (c == '@') {
                s.player = {x, y};
                s.grid[idx(x, y, s.w)] = '.';
            } else {
                s.grid[idx(x, y, s.w)] = c;

                if (c == 'T')
                    s.teleporters.push_back({x, y});
            }
        }
    }

    s.initialGrid = s.grid;
    s.initialPlayer = s.player;
    s.won = false;

    return in;
}

std::ostream& operator<<(std::ostream& out,
                         const Sokoban& s) {
    out << s.h << " " << s.w << "\n";

    for (unsigned y = 0; y < s.h; y++) {
        for (unsigned x = 0; x < s.w; x++)
            out << s.grid[idx(x, y, s.w)];
        out << "\n";
    }

    return out;
}

}  // namespace SB
