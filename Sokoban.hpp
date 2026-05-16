// Copyright 2026 Aanya Bharti

#pragma once

#include <iostream>
#include <stack>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace SB {

enum class Direction { Up, Down, Left, Right };

class Sokoban : public sf::Drawable {
 public:
  static const int TILE_SIZE = 64;

  Sokoban();
  explicit Sokoban(const std::string& filename);
  ~Sokoban() = default;

  unsigned int height() const;
  unsigned int width() const;

  sf::Vector2u size() const;
  sf::Vector2u windowSize() const;
  sf::Vector2u playerLoc() const;

  bool isWon() const;

  void movePlayer(Direction dir);
  void reset();

  // Extra credit
  void undo();
  void redo();

 protected:
  void draw(sf::RenderTarget& target,
            sf::RenderStates states) const override;

 private:
  // Grid dimensions
  unsigned w = 0;
  unsigned h = 0;

  // Level data
  std::vector<char> grid;
  std::vector<char> initialGrid;

  // Player state
  sf::Vector2u player{0, 0};
  sf::Vector2u initialPlayer{0, 0};

  // Teleporters (extra credit feature)
  std::vector<sf::Vector2u> teleporters;

  // Direction player is facing
  Direction facing = Direction::Down;

  // Graphics
  sf::Texture tiles;
  sf::Font font;
  mutable sf::Text moveText;
  sf::Text winText;

  // Audio
  sf::SoundBuffer winBuffer;
  sf::Sound winSound;

  // Game state
  unsigned moves = 0;
  bool won = false;

  // Undo / Redo stacks
  std::stack<std::vector<char>> undoGrid;
  std::stack<sf::Vector2u> undoPlayer;

  std::stack<std::vector<char>> redoGrid;
  std::stack<sf::Vector2u> redoPlayer;

  // File operators
  friend std::istream& operator>>(std::istream& in, Sokoban& s);
  friend std::ostream& operator<<(std::ostream& out,
                                  const Sokoban& s);
};

std::istream& operator>>(std::istream& in, Sokoban& s);
std::ostream& operator<<(std::ostream& out,
                         const Sokoban& s);

}  // namespace SB
