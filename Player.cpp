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
    // SIMPLE direction change
    if (glm::length(desiredDirection) > 0.0f && desiredDirection != direction)
    {
        glm::vec2 testPos = position + desiredDirection * 0.5f;
        int testX = (int)testPos.x;
        int testY = (int)testPos.y;
        
        if (testX >= 0 && testX < 28 && testY >= 0 && testY < 31 &&
            maze->IsWalkable(testX, testY))
        {
            direction = desiredDirection;
        }
    }
    
    // SIMPLE movement with SOLID collision
    if (glm::length(direction) > 0.0f)
    {
        glm::vec2 movement = direction * speed * dt;
        glm::vec2 newPos = position + movement;
        
        // Check the grid position we're moving to
        int gridX = (int)newPos.x;
        int gridY = (int)newPos.y;
        
        // Only move if the destination is walkable
        if (gridX >= 0 && gridX < 28 && gridY >= 0 && gridY < 31 &&
            maze->IsWalkable(gridX, gridY))
        {
            position = newPos;
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
    return glm::ivec2(position);
}
