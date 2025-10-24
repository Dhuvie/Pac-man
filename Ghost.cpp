#include "Ghost.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <unordered_set>
#include <unordered_map>

// Hash function for glm::ivec2 to use in unordered_map
struct ivec2Hash {
    std::size_t operator()(const glm::ivec2& v) const {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};

Ghost::Ghost(glm::vec2 startPos, glm::vec3 color, GhostAI aiType)
    : position(startPos), startPosition(startPos), direction(0.0f, -1.0f),
      color(color), aiType(aiType), active(false), frightened(false), eaten(false),
      baseSpeed(3.5f), speed(baseSpeed), pathUpdateTimer(0.0f),
      animationTimer(0.0f), animationFrame(0), pathIndex(0)
{
    targetTile = startPos;
}

void Ghost::Update(float dt, Maze* maze, glm::vec2 playerPos, 
                   glm::vec2 playerDir, bool frightened, int level)
{
    this->frightened = frightened;
    
    // AGGRESSIVE GHOSTS - SAME SPEED AS PLAYER
    if (frightened)
        speed = baseSpeed * 0.7f; // Slower when frightened so player can escape
    else
        speed = baseSpeed; // Same speed as player for fair chase
    
    // Update AI and pathfinding - ULTRA FREQUENT UPDATES FOR MAXIMUM AGGRESSION
    pathUpdateTimer += dt;
    if (pathUpdateTimer > 0.1f) // Update path 10 times per second for relentless pursuit
    {
        UpdateAI(maze, playerPos, playerDir, level);
        pathUpdateTimer = 0.0f;
    }
    
    // Update movement
    UpdateMovement(dt, maze);
    
    // Update animation
    animationTimer += dt;
    if (animationTimer > 0.1f)
    {
        animationFrame = (animationFrame + 1) % 4;
        animationTimer = 0.0f;
    }
}

void Ghost::UpdateReturn(float dt, Maze* maze)
{
    speed = baseSpeed * 2.0f; // Move faster when returning
    
    glm::ivec2 home = glm::ivec2(13, 14); // Ghost house position
    glm::ivec2 currentGrid = GetGridPosition();
    
    if (glm::distance(glm::vec2(currentGrid), glm::vec2(home)) < 0.5f)
    {
        eaten = false;
        position = startPosition;
        currentPath.clear();
        return;
    }
    
    // Update path to home
    pathUpdateTimer += dt;
    if (pathUpdateTimer > 0.3f)
    {
        currentPath = FindPathAStar(maze, currentGrid, home);
        pathIndex = 0;
        pathUpdateTimer = 0.0f;
    }
    
    UpdateMovement(dt, maze);
}

void Ghost::UpdateAI(Maze* maze, glm::vec2 playerPos, glm::vec2 playerDir, int level)
{
    glm::ivec2 currentGrid = GetGridPosition();
    glm::ivec2 playerGrid = glm::ivec2(playerPos);
    
    if (frightened)
    {
        // Run away from player - move to opposite corner
        glm::ivec2 scatterTarget = GetScatterTarget();
        glm::ivec2 awayTarget = currentGrid + (currentGrid - playerGrid);
        awayTarget = glm::clamp(awayTarget, glm::ivec2(0), glm::ivec2(27, 30));
        
        currentPath = FindPathDijkstra(maze, currentGrid, awayTarget);
        pathIndex = 0;
        return;
    }
    
    // ALL GHOSTS NOW CHASE PLAYER DIRECTLY - MAXIMUM AGGRESSION
    switch (aiType)
    {
        case GhostAI::AGGRESSIVE:
        {
            // Red Ghost: Direct chase using Dijkstra's algorithm
            currentPath = FindPathDijkstra(maze, currentGrid, playerGrid);
            pathIndex = 0;
            break;
        }
        
        case GhostAI::AMBUSH:
        {
            // Pink Ghost: Also chase directly (no more ambush, pure aggression)
            currentPath = FindPathDijkstra(maze, currentGrid, playerGrid);
            pathIndex = 0;
            break;
        }
        
        case GhostAI::PATROL:
        {
            // Cyan Ghost: Also chase directly (no more patrol, pure aggression)
            currentPath = FindPathDijkstra(maze, currentGrid, playerGrid);
            pathIndex = 0;
            break;
        }
        
        case GhostAI::RANDOM:
        {
            // Orange Ghost: Also chase directly (no more random, pure aggression)
            currentPath = FindPathDijkstra(maze, currentGrid, playerGrid);
            pathIndex = 0;
            break;
        }
    }
}

void Ghost::UpdateMovement(float dt, Maze* maze)
{
    if (currentPath.empty() || pathIndex >= currentPath.size())
        return;
    
    glm::vec2 targetPos = glm::vec2(currentPath[pathIndex]) + glm::vec2(0.5f);
    glm::vec2 toTarget = targetPos - position;
    float distance = glm::length(toTarget);
    
    if (distance < 0.1f)
    {
        pathIndex++;
        if (pathIndex < currentPath.size())
        {
            targetPos = glm::vec2(currentPath[pathIndex]) + glm::vec2(0.5f);
            toTarget = targetPos - position;
        }
    }
    
    if (glm::length(toTarget) > 0.0f)
    {
        direction = glm::normalize(toTarget);
        glm::vec2 movement = direction * speed * dt;
        
        // Check if movement is valid with bounds checking
        glm::vec2 newPos = position + movement;
        int gridX = (int)std::floor(newPos.x);
        int gridY = (int)std::floor(newPos.y);
        
        // Ensure bounds and walkability
        if (gridX >= 0 && gridX < 28 && gridY >= 0 && gridY < 31 && 
            maze->IsWalkable(gridX, gridY))
        {
            position = newPos;
        }
        else
        {
            // If stuck, try to find new path immediately
            pathIndex = currentPath.size(); // Force path recalculation
        }
    }
    
    // Wrap around screen edges
    if (position.x < 0.0f) position.x = 27.0f;
    if (position.x > 27.0f) position.x = 0.0f;
}

std::vector<glm::ivec2> Ghost::FindPathDijkstra(Maze* maze, glm::ivec2 start, glm::ivec2 goal)
{
    std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet;
    std::unordered_map<glm::ivec2, float, ivec2Hash> costSoFar;
    std::unordered_map<glm::ivec2, glm::ivec2, ivec2Hash> cameFrom;
    
    PathNode startNode;
    startNode.position = start;
    startNode.cost = 0.0f;
    startNode.heuristic = 0.0f;
    startNode.parent = start;
    
    openSet.push(startNode);
    costSoFar[start] = 0.0f;
    cameFrom[start] = start;
    
    while (!openSet.empty())
    {
        PathNode current = openSet.top();
        openSet.pop();
        
        if (current.position == goal)
        {
            // Reconstruct path
            std::vector<glm::ivec2> path;
            glm::ivec2 pos = goal;
            
            while (pos != start)
            {
                path.push_back(pos);
                pos = cameFrom[pos];
            }
            
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        for (const auto& neighbor : GetNeighbors(maze, current.position))
        {
            float newCost = costSoFar[current.position] + 1.0f;
            
            if (costSoFar.find(neighbor) == costSoFar.end() || newCost < costSoFar[neighbor])
            {
                costSoFar[neighbor] = newCost;
                cameFrom[neighbor] = current.position;
                
                PathNode neighborNode;
                neighborNode.position = neighbor;
                neighborNode.cost = newCost;
                neighborNode.heuristic = 0.0f; // Pure Dijkstra (no heuristic)
                neighborNode.parent = current.position;
                
                openSet.push(neighborNode);
            }
        }
    }
    
    return std::vector<glm::ivec2>(); // No path found
}

std::vector<glm::ivec2> Ghost::FindPathBacktracking(Maze* maze, glm::ivec2 start, 
                                                      glm::ivec2 goal, int maxDepth)
{
    std::vector<glm::ivec2> path;
    std::vector<std::vector<bool>> visited(28, std::vector<bool>(31, false));
    
    if (BacktrackRecursive(maze, start, goal, path, visited, 0, maxDepth))
    {
        return path;
    }
    
    return std::vector<glm::ivec2>();
}

bool Ghost::BacktrackRecursive(Maze* maze, glm::ivec2 current, glm::ivec2 goal,
                               std::vector<glm::ivec2>& path, 
                               std::vector<std::vector<bool>>& visited,
                               int depth, int maxDepth)
{
    // Base cases
    if (current == goal)
    {
        path.push_back(current);
        return true;
    }
    
    if (depth >= maxDepth)
        return false;
    
    if (current.x < 0 || current.x >= 28 || current.y < 0 || current.y >= 31)
        return false;
    
    if (visited[current.x][current.y])
        return false;
    
    if (!maze->IsWalkable(current.x, current.y))
        return false;
    
    // Mark as visited
    visited[current.x][current.y] = true;
    path.push_back(current);
    
    // Try all four directions (prioritize towards goal)
    std::vector<glm::ivec2> directions = {
        glm::ivec2(0, -1),  // Up
        glm::ivec2(0, 1),   // Down
        glm::ivec2(-1, 0),  // Left
        glm::ivec2(1, 0)    // Right
    };
    
    // Sort directions by distance to goal (greedy heuristic)
    std::sort(directions.begin(), directions.end(), 
        [&](const glm::ivec2& a, const glm::ivec2& b) {
            float distA = Heuristic(current + a, goal);
            float distB = Heuristic(current + b, goal);
            return distA < distB;
        });
    
    for (const auto& dir : directions)
    {
        glm::ivec2 next = current + dir;
        
        if (BacktrackRecursive(maze, next, goal, path, visited, depth + 1, maxDepth))
        {
            return true;
        }
    }
    
    // Backtrack
    path.pop_back();
    return false;
}

std::vector<glm::ivec2> Ghost::FindPathAStar(Maze* maze, glm::ivec2 start, glm::ivec2 goal)
{
    std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet;
    std::unordered_map<glm::ivec2, float, ivec2Hash> costSoFar;
    std::unordered_map<glm::ivec2, glm::ivec2, ivec2Hash> cameFrom;
    
    PathNode startNode;
    startNode.position = start;
    startNode.cost = 0.0f;
    startNode.heuristic = Heuristic(start, goal);
    startNode.parent = start;
    
    openSet.push(startNode);
    costSoFar[start] = 0.0f;
    cameFrom[start] = start;
    
    while (!openSet.empty())
    {
        PathNode current = openSet.top();
        openSet.pop();
        
        if (current.position == goal)
        {
            std::vector<glm::ivec2> path;
            glm::ivec2 pos = goal;
            
            while (pos != start)
            {
                path.push_back(pos);
                pos = cameFrom[pos];
            }
            
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        for (const auto& neighbor : GetNeighbors(maze, current.position))
        {
            float newCost = costSoFar[current.position] + 1.0f;
            
            if (costSoFar.find(neighbor) == costSoFar.end() || newCost < costSoFar[neighbor])
            {
                costSoFar[neighbor] = newCost;
                cameFrom[neighbor] = current.position;
                
                PathNode neighborNode;
                neighborNode.position = neighbor;
                neighborNode.cost = newCost;
                neighborNode.heuristic = Heuristic(neighbor, goal);
                neighborNode.parent = current.position;
                
                openSet.push(neighborNode);
            }
        }
    }
    
    return std::vector<glm::ivec2>();
}

float Ghost::Heuristic(glm::ivec2 a, glm::ivec2 b)
{
    // Manhattan distance
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

glm::ivec2 Ghost::GetGridPosition() const
{
    return glm::ivec2(position);
}

std::vector<glm::ivec2> Ghost::GetNeighbors(Maze* maze, glm::ivec2 pos)
{
    std::vector<glm::ivec2> neighbors;
    std::vector<glm::ivec2> directions = {
        glm::ivec2(0, -1), glm::ivec2(0, 1),
        glm::ivec2(-1, 0), glm::ivec2(1, 0)
    };
    
    for (const auto& dir : directions)
    {
        glm::ivec2 neighbor = pos + dir;
        
        if (neighbor.x >= 0 && neighbor.x < 28 && 
            neighbor.y >= 0 && neighbor.y < 31 &&
            maze->IsWalkable(neighbor.x, neighbor.y))
        {
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

glm::ivec2 Ghost::GetScatterTarget()
{
    // Each ghost has a different scatter corner
    switch (aiType)
    {
        case GhostAI::AGGRESSIVE: return glm::ivec2(25, 0);   // Top right
        case GhostAI::AMBUSH:     return glm::ivec2(2, 0);    // Top left
        case GhostAI::PATROL:     return glm::ivec2(27, 30);  // Bottom right
        case GhostAI::RANDOM:     return glm::ivec2(0, 30);   // Bottom left
        default:                  return glm::ivec2(13, 14);
    }
}

void Ghost::Render(Renderer* renderer, bool powerUpActive)
{
    glm::vec2 renderPos = position * 32.0f;
    float size = 28.0f;
    
    glm::vec3 renderColor = color;
    if (frightened && !eaten)
    {
        renderColor = glm::vec3(0.2f, 0.2f, 0.8f); // Blue when frightened
        
        // Flash white when power-up is about to end
        if (powerUpActive && (int)(animationTimer * 10) % 2 == 0)
            renderColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    else if (eaten)
    {
        renderColor = glm::vec3(0.5f, 0.5f, 0.5f); // Gray when eaten
        size = 20.0f;
    }
    
    // Draw ghost body
    renderer->DrawCircle(renderPos, size / 2.0f, renderColor);
    
    // Draw eyes (unless eaten)
    if (!eaten)
    {
        glm::vec3 eyeColor = frightened ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(1.0f);
        renderer->DrawCircle(renderPos + glm::vec2(-6, -4), 4.0f, eyeColor);
        renderer->DrawCircle(renderPos + glm::vec2(6, -4), 4.0f, eyeColor);
        
        // Pupils
        glm::vec2 pupilOffset = direction * 2.0f;
        renderer->DrawCircle(renderPos + glm::vec2(-6, -4) + pupilOffset, 2.0f, glm::vec3(0.0f));
        renderer->DrawCircle(renderPos + glm::vec2(6, -4) + pupilOffset, 2.0f, glm::vec3(0.0f));
    }
}

void Ghost::Reset(glm::vec2 pos)
{
    position = pos;
    startPosition = pos;
    direction = glm::vec2(0.0f, -1.0f);
    active = false;
    frightened = false;
    eaten = false;
    speed = baseSpeed;
    currentPath.clear();
    pathIndex = 0;
}

void Ghost::SetEaten(bool val)
{
    eaten = val;
    if (eaten)
    {
        frightened = false;
        currentPath.clear();
    }
}
