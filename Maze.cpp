#include "Maze.h"
#include <cmath>

Maze::Maze()
    : totalDots(0), dotsEaten(0), dotAnimTimer(0.0f), pelletAnimTimer(0.0f)
{
    tiles.resize(WIDTH, std::vector<int>(HEIGHT, EMPTY));
    dots.resize(WIDTH, std::vector<bool>(HEIGHT, false));
    powerPellets.resize(WIDTH, std::vector<bool>(HEIGHT, false));
    
    InitializeMaze();
}

void Maze::InitializeMaze()
{
    // BEST 28x31 maze layout - perfect for window
    // 1 = wall, 0 = empty, 2 = dot, 3 = power pellet
    int layout[31][28] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
        {1,3,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,3,1},
        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
        {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
        {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
        {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,1,1,1,0,1,1,0,1,1,1,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,1,1,1,4,4,1,1,1,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,1,4,4,4,4,4,4,1,0,1,1,2,1,1,1,1,1,1},
        {0,0,0,0,0,0,2,0,0,0,1,4,4,4,4,4,4,1,0,0,0,2,0,0,0,0,0,0},
        {1,1,1,1,1,1,2,1,1,0,1,4,4,4,4,4,4,1,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
        {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
        {1,3,2,2,1,1,2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,1,1,2,2,3,1},
        {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
        {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
        {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
        {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
        {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
    
    totalDots = 0;
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            tiles[x][y] = layout[y][x];
            
            if (layout[y][x] == DOT)
            {
                dots[x][y] = true;
                totalDots++;
                tiles[x][y] = EMPTY;
            }
            else if (layout[y][x] == POWER_PELLET)
            {
                powerPellets[x][y] = true;
                totalDots++;
                tiles[x][y] = EMPTY;
            }
            else
            {
                dots[x][y] = false;
                powerPellets[x][y] = false;
            }
        }
    }
    
    dotsEaten = 0;
}

void Maze::Render(Renderer* renderer)
{
    dotAnimTimer += 0.016f;
    pelletAnimTimer += 0.016f;
    
    float tileSize = 32.0f;
    
    // Render walls with gradient effect
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            glm::vec2 pos(x * tileSize, y * tileSize);
            
            if (tiles[x][y] == WALL)
            {
                // Blue gradient walls
                float gradient = 0.3f + 0.2f * std::sin((x + y) * 0.5f);
                glm::vec3 wallColor(0.0f, 0.0f + gradient, 0.5f + gradient);
                renderer->DrawRect(pos, glm::vec2(tileSize), wallColor);
                
                // Add border for depth
                renderer->DrawRectOutline(pos, glm::vec2(tileSize), 
                    glm::vec3(0.2f, 0.4f, 1.0f), 2.0f);
            }
            else if (tiles[x][y] == GHOST_HOUSE)
            {
                // Pink ghost house
                renderer->DrawRect(pos, glm::vec2(tileSize), 
                    glm::vec3(0.3f, 0.0f, 0.3f));
            }
        }
    }
    
    // Render dots with pulsing animation
    float dotPulse = 1.0f + 0.2f * std::sin(dotAnimTimer * 3.0f);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            glm::vec2 pos(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2);
            
            if (dots[x][y])
            {
                renderer->DrawCircle(pos, 3.0f * dotPulse, 
                    glm::vec3(1.0f, 0.9f, 0.7f));
            }
            else if (powerPellets[x][y])
            {
                // Animated power pellets
                float pelletPulse = 1.0f + 0.5f * std::sin(pelletAnimTimer * 4.0f);
                float glow = 0.5f + 0.5f * std::sin(pelletAnimTimer * 5.0f);
                
                renderer->DrawCircle(pos, 8.0f * pelletPulse, 
                    glm::vec3(1.0f, glow, 0.5f));
                
                // Outer glow
                renderer->DrawCircle(pos, 12.0f * pelletPulse, 
                    glm::vec3(1.0f, 0.5f, 0.0f) * 0.3f);
            }
        }
    }
}

void Maze::Reset()
{
    InitializeMaze();
}

bool Maze::IsWalkable(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return true; // Allow wrapping
    
    return tiles[x][y] != WALL;
}

bool Maze::HasDot(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return false;
    
    return dots[x][y];
}

bool Maze::HasPowerPellet(int x, int y) const
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return false;
    
    return powerPellets[x][y];
}

void Maze::RemoveDot(int x, int y)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && dots[x][y])
    {
        dots[x][y] = false;
        dotsEaten++;
    }
}

void Maze::RemovePowerPellet(int x, int y)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && powerPellets[x][y])
    {
        powerPellets[x][y] = false;
        dotsEaten++;
    }
}

bool Maze::AllDotsEaten() const
{
    return dotsEaten >= totalDots;
}

glm::vec2 Maze::GridToWorld(int x, int y) const
{
    return glm::vec2(x * 32.0f + 16.0f, y * 32.0f + 16.0f);
}
