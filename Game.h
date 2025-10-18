#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "Renderer.h"
#include "Player.h"
#include "Ghost.h"
#include "Maze.h"
#include "ParticleSystem.h"

enum GameState {
    GAME_MENU,
    GAME_ACTIVE,
    GAME_PAUSED,
    GAME_WIN,
    GAME_OVER
};

class Game
{
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    
    int Score;
    int HighScore;
    int Lives;
    int Level;
    
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    std::unique_ptr<Maze> maze;
    std::unique_ptr<ParticleSystem> particles;
    
    float stateTimer;
    float ghostReleaseTimer;
    float powerUpTimer;
    bool powerUpActive;
    int ghostsEaten;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void Reset();
    void NextLevel();
    
    void LoadHighScore();
    void SaveHighScore();
    
    void EatDot(int x, int y);
    void EatPowerPellet();
    void EatGhost(Ghost* ghost);
    void LoseLife();
    
    bool CheckCollision(const glm::vec2& pos1, const glm::vec2& pos2, float threshold = 0.3f);
};

#endif
