#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <glm/glm.hpp>
#include "Renderer.h"

enum TileType {
    EMPTY = 0,
    WALL = 1,
    DOT = 2,
    POWER_PELLET = 3,
    GHOST_HOUSE = 4
};

class Maze
{
public:
    Maze();
    
    void Render(Renderer* renderer);
    void Reset();
    
    bool IsWalkable(int x, int y) const;
    bool HasDot(int x, int y) const;
    bool HasPowerPellet(int x, int y) const;
    void RemoveDot(int x, int y);
    void RemovePowerPellet(int x, int y);
    bool AllDotsEaten() const;
    
    glm::vec2 GridToWorld(int x, int y) const;
    
    static const int WIDTH = 28;  // Perfect window size
    static const int HEIGHT = 31; // Perfect window size
    
private:
    std::vector<std::vector<int>> tiles;
    std::vector<std::vector<bool>> dots;
    std::vector<std::vector<bool>> powerPellets;
    
    int totalDots;
    int dotsEaten;
    
    float dotAnimTimer;
    float pelletAnimTimer;
    
    void InitializeMaze();
};

#endif
