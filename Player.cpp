#include "Player.h"
#include <cmath>

Player::Player(glm::vec2 startPos)
    : position(startPos), startPosition(startPos),
      direction(0.0f, 0.0f), desiredDirection(0.0f, 0.0f),
      speed(5.0f), animationTimer(0.0f), mouthAngle(0.0f), mouthOpening(true)
{
}

void Player::Update(float dt, Maze* maze)
{
    // SMART direction change - only turn if the new direction is actually walkable
    if (glm::length(desiredDirection) > 0.0f && desiredDirection != direction)
    {
        // Check if we can actually move in the desired direction
        glm::vec2 testPos = position + desiredDirection * 0.8f;
        bool canTurn = true;
        float checkRadius = 0.4f;
        
        // Check multiple points to ensure the turn is safe
        glm::vec2 checkPoints[] = {
            testPos, // center
            testPos + glm::vec2(-checkRadius, -checkRadius), // corners
            testPos + glm::vec2(checkRadius, -checkRadius),
            testPos + glm::vec2(-checkRadius, checkRadius),
            testPos + glm::vec2(checkRadius, checkRadius)
        };
        
        for (const auto& point : checkPoints)
        {
            int gridX = (int)std::floor(point.x);
            int gridY = (int)std::floor(point.y);
            
            if (gridX < 0 || gridX >= 28 || gridY < 0 || gridY >= 31 ||
                !maze->IsWalkable(gridX, gridY))
            {
                canTurn = false;
                break;
            }
        }
        
        // Only change direction if the turn is completely safe
        if (canTurn)
        {
            direction = desiredDirection;
        }
        // If can't turn, keep current direction - no freezing!
    }
    
    // BULLETPROOF movement with SOLID collision
    if (glm::length(direction) > 0.0f)
    {
        glm::vec2 movement = direction * speed * dt;
        glm::vec2 newPos = position + movement;
        
        // Check multiple points around Pac-Man to prevent wall penetration
        bool canMove = true;
        float checkRadius = 0.4f;
        
        // Check center and 4 corners
        glm::vec2 checkPoints[] = {
            newPos, // center
            newPos + glm::vec2(-checkRadius, -checkRadius), // top-left
            newPos + glm::vec2(checkRadius, -checkRadius),  // top-right
            newPos + glm::vec2(-checkRadius, checkRadius),  // bottom-left
            newPos + glm::vec2(checkRadius, checkRadius)    // bottom-right
        };
        
        for (const auto& point : checkPoints)
        {
            int gridX = (int)std::floor(point.x);
            int gridY = (int)std::floor(point.y);
            
            if (gridX < 0 || gridX >= 28 || gridY < 0 || gridY >= 31 ||
                !maze->IsWalkable(gridX, gridY))
            {
                canMove = false;
                break;
            }
        }
        
        if (canMove)
        {
            position = newPos;
        }
        else
        {
            // If can't move forward, stop moving to prevent getting stuck
            direction = glm::vec2(0.0f, 0.0f);
        }
    }
    
    // Wrap around screen edges for 28x31 maze
    if (position.x < 0.0f) position.x = 27.0f;
    if (position.x > 27.0f) position.x = 0.0f;
    
    // Update mouth animation
    animationTimer += dt;
    if (animationTimer > 0.05f)
    {
        if (mouthOpening)
        {
            mouthAngle += 5.0f;
            if (mouthAngle >= 45.0f)
            {
                mouthAngle = 45.0f;
                mouthOpening = false;
            }
        }
        else
        {
            mouthAngle -= 5.0f;
            if (mouthAngle <= 0.0f)
            {
                mouthAngle = 0.0f;
                mouthOpening = true;
            }
        }
        animationTimer = 0.0f;
    }
}

void Player::Render(Renderer* renderer)
{
    glm::vec2 renderPos = position * 32.0f;
    float size = 28.0f;
    
    // Calculate rotation based on direction
    float rotation = 0.0f;
    if (direction.x > 0.0f) rotation = 0.0f;
    else if (direction.x < 0.0f) rotation = 180.0f;
    else if (direction.y > 0.0f) rotation = 90.0f;
    else if (direction.y < 0.0f) rotation = 270.0f;
    
    // Draw Pac-Man with animated mouth
    renderer->DrawPacman(renderPos, size / 2.0f, rotation, mouthAngle,
                        glm::vec3(1.0f, 1.0f, 0.0f));
}

void Player::Reset(glm::vec2 pos)
{
    position = pos;
    startPosition = pos;
    direction = glm::vec2(0.0f, 0.0f);
    desiredDirection = glm::vec2(0.0f, 0.0f);
    mouthAngle = 0.0f;
    mouthOpening = true;
}

glm::ivec2 Player::GetGridPosition() const
{
    // Round to nearest grid position for accurate dot collection
    return glm::ivec2(std::round(position.x), std::round(position.y));
}
