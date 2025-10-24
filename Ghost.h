#ifndef GHOST_H
#define GHOST_H

#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <stack>
#include "Maze.h"
#include "Renderer.h"

enum class GhostAI {
    AGGRESSIVE,  // Uses Dijkstra to chase player directly
    AMBUSH,      // Predicts player movement and ambushes
    PATROL,      // Patrols and uses backtracking
    RANDOM       // Random movement with occasional chasing
};

struct PathNode {
    glm::ivec2 position;
    float cost;
    float heuristic;
    glm::ivec2 parent;
    
    float GetTotal() const { return cost + heuristic; }
    
    bool operator>(const PathNode& other) const {
        return GetTotal() > other.GetTotal();
    }
};

class Ghost
{
public:
    Ghost(glm::vec2 startPos, glm::vec3 color, GhostAI aiType);
    
    void Update(float dt, Maze* maze, glm::vec2 playerPos, 
                glm::vec2 playerDir, bool frightened, int level);
    void UpdateReturn(float dt, Maze* maze);
    void Render(Renderer* renderer, bool powerUpActive);
    void Reset(glm::vec2 pos);
    
    glm::vec2 GetPosition() const { return position; }
    bool IsActive() const { return active; }
    void SetActive(bool val) { active = val; }
    bool IsEaten() const { return eaten; }
    void SetEaten(bool val);
    void SetFrightened(bool val) { frightened = val; }
    
private:
    glm::vec2 position;
    glm::vec2 startPosition;
    glm::vec2 direction;
    glm::vec2 targetTile;
    glm::vec3 color;
    
    GhostAI aiType;
    bool active;
    bool frightened;
    bool eaten;
    
    float speed;
    float baseSpeed;
    float pathUpdateTimer;
    float animationTimer;
    int animationFrame;
    
    std::vector<glm::ivec2> currentPath;
    int pathIndex;
    
    // AI Methods
    void UpdateAI(Maze* maze, glm::vec2 playerPos, glm::vec2 playerDir, int level);
    void UpdateMovement(float dt, Maze* maze);
    
    // Dijkstra's Algorithm for pathfinding
    std::vector<glm::ivec2> FindPathDijkstra(Maze* maze, glm::ivec2 start, glm::ivec2 goal);
    
    // Backtracking for patrol behavior
    std::vector<glm::ivec2> FindPathBacktracking(Maze* maze, glm::ivec2 start, 
                                                   glm::ivec2 goal, int maxDepth);
    bool BacktrackRecursive(Maze* maze, glm::ivec2 current, glm::ivec2 goal,
                           std::vector<glm::ivec2>& path, 
                           std::vector<std::vector<bool>>& visited, int depth, int maxDepth);
    
    // A* for eaten ghosts returning home
    std::vector<glm::ivec2> FindPathAStar(Maze* maze, glm::ivec2 start, glm::ivec2 goal);
    
    // Helper methods
    float Heuristic(glm::ivec2 a, glm::ivec2 b);
    glm::ivec2 GetGridPosition() const;
    std::vector<glm::ivec2> GetNeighbors(Maze* maze, glm::ivec2 pos);
    glm::ivec2 GetScatterTarget();
};

#endif
