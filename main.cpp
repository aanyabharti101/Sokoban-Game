// Copyright 2026 Aanya Bharti
#include <fstream>
#include <iostream>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./Sokoban <level_file>\n";
        return 1;
    }

    SB::Sokoban game(argv[1]);

    sf::RenderWindow window(
        sf::VideoMode(game.windowSize()),
        "Sokoban");

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>())
                window.close();

            if (const auto* key =
                    event.getIf<sf::Event::KeyPressed>()) {
                using Key = sf::Keyboard::Key;

                if (key->code == Key::Escape)
                    window.close();

                else if (key->code == Key::Up || key->code == Key::W)
                    game.movePlayer(SB::Direction::Up);

                else if (key->code == Key::Down || key->code == Key::S)
                    game.movePlayer(SB::Direction::Down);

                else if (key->code == Key::Left || key->code == Key::A)
                    game.movePlayer(SB::Direction::Left);

                else if (key->code == Key::Right || key->code == Key::D)
                    game.movePlayer(SB::Direction::Right);

                else if (key->code == Key::R)
                    game.reset();

                else if (key->code == Key::Z)
                    game.undo();

                else if (key->code == Key::Y)
                    game.redo();
            }
        }

        window.clear();
        window.draw(game);
        window.display();
    }

    return 0;
}
