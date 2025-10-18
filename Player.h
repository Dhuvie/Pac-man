#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "Maze.h"
#include "Renderer.h"

class Player
{
public:
    Player(glm::vec2 startPos);
    
    void Update(float dt, Maze* maze);
    void Render(Renderer* renderer);
    void Reset(glm::vec2 pos);
    
    void SetDirection(glm::vec2 dir) { desiredDirection = dir; }
    glm::vec2 GetPosition() const { return position; }
    glm::vec2 GetDirection() const { return direction; }
    glm::ivec2 GetGridPosition() const;
    
private:
    glm::vec2 position;
    glm::vec2 startPosition;
    glm::vec2 direction;
    glm::vec2 desiredDirection;
    
    float speed;
    float animationTimer;
    float mouthAngle;
    bool mouthOpening;
};

#endif
