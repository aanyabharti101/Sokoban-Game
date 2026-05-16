# PS3: Sokoban

## Contact
Name: Aanya Bharti
Section: 203
Time to Complete: 8 hours

## Description
This project loads a Sokoban level from a text file and displays it using SFML. The level file specifies the grid dimensions and the initial layout using symbols for walls, floor, player, crates, and storage locations.

The program reads the level using an overloaded >> operator, stores the layout internally, and renders it using a tilesheet texture with 64×64 pixel tiles. The window automatically resizes based on the level dimensions.

### Features
-Reads a level file using an overloaded operator>>.
-Stores the level in a row-major 1D grid (std::vector<char>).
-Separately tracks player position.
-Draws the board using a tilesheet texture (64×64 tiles).
-Draws floor/walls first, then storage/crates, then player on top.
-Window size is computed from width * 64 and height * 64.
-Includes a move counter (extra credit).

#### Part A [Level Loading and Rendering]
-The level loads correctly.
-The board renders correctly.
-Player position is extracted from the file.
-Window size matches the level dimensions.
-Non-square levels render properly.
-Layer ordering is correct.
-movePlayer() and isWon() are implemented as required but behavior is undefined for Part A per the assignment instructions.

#### Part B [Gameplay Mechanics]
## Basic Movement Implementation
Movement is handled in:
Movement steps:
1. Convert the Direction enum into `(dx, dy)` values.
2. Compute the potential new player position.
3. Check boundary conditions.
4. Check collision conditions.
5. Update player position if the move is valid.
6. Increment the move counter.
7. Check win condition.

Movement is disabled once the player wins.
The player sprite changes direction based on the most recent move.

## Collision Handling
### Wall Collision
After confirming the position is within bounds, the tile at `(nx, ny)` is examined.
If the tile is `#`, the movement is cancelled and the player position remains unchanged.

### Boundary Detection
Before accessing the grid, the program computes the candidate position `(nx, ny)` using the directional offset `(dx, dy)`.
The move is immediately aborted if `(nx, ny)` is outside the valid grid range.
This prevents out-of-bounds indexing.

### Box Collision
If the tile at `(nx, ny)` contains a box:
1. The tile beyond the box `(bx, by)` is computed using the same directional offset.
2. The program verifies `(bx, by)` is within bounds.
3. If the beyond tile is a wall (`#`) or another box, the move is cancelled.
4. Otherwise:
   - The box is moved forward into `(bx, by)`.
   - The current box tile is updated.
   - The player moves into the box’s previous location.

Only one box may be pushed at a time, and boxes cannot be pulled.


### Lambda and <algorithm> Usage
The isWon() function uses std::count_if from <algorithm>
with lambda expressions to count boxes and storage tiles:

[](char c) { return c == 'A' || c == '1'; }

This allows concise computation of the win condition while
satisfying the assignment requirement to use a lambda
expression as a parameter.

### Memory
The level is stored in a std::vector<char> in row-major order.
Index math used:
i = x + y * width

The player location is stored separately as an sf::Vector2u.
An initial copy of the grid and player position is stored to support the reset() function.
No raw pointers are used.
No manual dynamic memory allocation is used.
All SFML objects (texture, font, text) are stored as class members, so there are no memory leaks.


### Issues
The biggest challenges were:
Correctly mapping the sprite sheet row/column coordinates.
Ensuring drawing order (floor first, then storage/crates, then player).
Adjusting for SFML 3’s requirement that sf::Text must be initialized in the constructor initializer list.
Debugging tile rectangle offsets.
Once layering and coordinates were correct, rendering behaved as expected.

### Extra Credit(PART A)
Move counter is implemented and shown in the game window:
The counter increments every time movePlayer() is called.
The counter resets when reset() is called.
The counter is drawn in the top-left corner of the game window using sf::Text.
The screenshot included in this submission shows the move counter displayed during gameplay.

### Extra Credit(PART B)
 **Directional Player Animation (+2):
  The player sprite updates texture based on the last movement direction.

- **Victory Sound (+2):
  A short `win.wav` sound plays when the win condition is reached.

- **New Feature: Teleporters (+3):
  Tile `T` teleports the player to its paired location.
  Only activates when exactly two teleporters exist.

- **Undo / Redo (+4 total):
  `Z` undoes the previous move.
  `Y` redoes an undone move.
  Full grid state and player position are stored using stacks.


## Acknowledgements
Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban
Font Used: Roboto (Open Font License): https://fonts.google.com/specimen/Roboto
Level files were provided as part of the course assignment materials.
SFML documentation was referenced for rendering and font usage.
Victory sound from MyInstants by user: Jaredvaldez04 for win.wav file

## Screenshot
![Program Output](screenshot.png)

The screenshot shows program running in the SFML window. This screenshot shows the game in a completed (win) state. The move counter is displayed in the top-left corner and updates after each valid move. The “You Win!” message appears once all required boxes are placed on storage locations, and movement is disabled at that point. The player sprite updates to face the direction of the most recent movement. This level include my extra credit: new game play feature which is teleporters(denoted by T and appears as a yellow token as seen on screenshot).
