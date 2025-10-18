#include "Game.h"
#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height),
      Score(0), HighScore(0), Lives(3), Level(1),
      stateTimer(0.0f), ghostReleaseTimer(0.0f), 
      powerUpTimer(0.0f), powerUpActive(false), ghostsEaten(0)
{
}

Game::~Game()
{
    SaveHighScore();
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    
    // Configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width),
        static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    
    // Initialize renderer
    renderer = std::make_unique<Renderer>(ResourceManager::GetShader("sprite"));
    
    // Initialize maze
    maze = std::make_unique<Maze>();
    
    // Initialize player - GUARANTEED WALKABLE POSITION (row 29 is ALL dots)
    player = std::make_unique<Player>(glm::vec2(13.0f, 29.0f));
    
    // Initialize ghosts with different AI behaviors - PERFECT positions
    ghosts.push_back(std::make_unique<Ghost>(glm::vec2(13.5f, 11.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f), GhostAI::AGGRESSIVE)); // Blinky - Red - Aggressive
    ghosts.push_back(std::make_unique<Ghost>(glm::vec2(11.5f, 14.0f), 
        glm::vec3(1.0f, 0.75f, 0.8f), GhostAI::AMBUSH)); // Pinky - Pink - Ambush
    ghosts.push_back(std::make_unique<Ghost>(glm::vec2(13.5f, 14.0f), 
        glm::vec3(0.0f, 1.0f, 1.0f), GhostAI::PATROL)); // Inky - Cyan - Patrol
    ghosts.push_back(std::make_unique<Ghost>(glm::vec2(15.5f, 14.0f), 
        glm::vec3(1.0f, 0.65f, 0.0f), GhostAI::RANDOM)); // Clyde - Orange - Random
    
    // Initialize particle system
    particles = std::make_unique<ParticleSystem>(ResourceManager::GetShader("particle"), 1000);
    
    // Load high score
    LoadHighScore();
    
    State = GAME_MENU;
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_MENU)
    {
        if (Keys[GLFW_KEY_ENTER])
        {
            State = GAME_ACTIVE;
            Reset();
        }
    }
    else if (State == GAME_ACTIVE)
    {
        if (Keys[GLFW_KEY_P])
        {
            State = GAME_PAUSED;
        }
        
        // Player movement
        glm::vec2 newDir(0.0f);
        if (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP])
            newDir = glm::vec2(0.0f, -1.0f);
        if (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN])
            newDir = glm::vec2(0.0f, 1.0f);
        if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT])
            newDir = glm::vec2(-1.0f, 0.0f);
        if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT])
            newDir = glm::vec2(1.0f, 0.0f);
        
        if (glm::length(newDir) > 0.0f)
            player->SetDirection(newDir);
    }
    else if (State == GAME_PAUSED)
    {
        if (Keys[GLFW_KEY_P])
        {
            State = GAME_ACTIVE;
            Keys[GLFW_KEY_P] = false;
        }
    }
    else if (State == GAME_OVER || State == GAME_WIN)
    {
        if (Keys[GLFW_KEY_ENTER])
        {
            State = GAME_MENU;
        }
    }
}

void Game::Update(float dt)
{
    ProcessInput(dt);
    
    if (State != GAME_ACTIVE)
        return;
    
    // Update power-up timer
    if (powerUpActive)
    {
        powerUpTimer -= dt;
        if (powerUpTimer <= 0.0f)
        {
            powerUpActive = false;
            for (auto& ghost : ghosts)
                ghost->SetFrightened(false);
        }
    }
    
    // Update player
    player->Update(dt, maze.get());
    
    // Check dot collection
    glm::ivec2 gridPos = player->GetGridPosition();
    if (maze->HasDot(gridPos.x, gridPos.y))
    {
        EatDot(gridPos.x, gridPos.y);
    }
    if (maze->HasPowerPellet(gridPos.x, gridPos.y))
    {
        EatPowerPellet();
        maze->RemovePowerPellet(gridPos.x, gridPos.y);
    }
    
    // Release ghosts over time
    ghostReleaseTimer += dt;
    if (ghostReleaseTimer > 2.0f)
    {
        for (auto& ghost : ghosts)
        {
            if (!ghost->IsActive())
            {
                ghost->SetActive(true);
                ghostReleaseTimer = 0.0f;
                break;
            }
        }
    }
    
    // Update ghosts with advanced AI
    for (auto& ghost : ghosts)
    {
        if (ghost->IsActive() && !ghost->IsEaten())
        {
            ghost->Update(dt, maze.get(), player->GetPosition(), 
                         player->GetDirection(), powerUpActive, Level);
        }
        else if (ghost->IsEaten())
        {
            ghost->UpdateReturn(dt, maze.get());
        }
        
        // Check collision with player
        if (ghost->IsActive() && CheckCollision(player->GetPosition(), ghost->GetPosition()))
        {
            if (powerUpActive && !ghost->IsEaten())
            {
                EatGhost(ghost.get());
            }
            else if (!ghost->IsEaten())
            {
                LoseLife();
            }
        }
    }
    
    // Update particles
    particles->Update(dt);
    
    // Check win condition
    if (maze->AllDotsEaten())
    {
        NextLevel();
    }
}

void Game::Render()
{
    if (State == GAME_MENU)
    {
        renderer->DrawText("PAC-MAN", glm::vec2(Width / 2 - 150, Height / 2 - 100), 
            2.5f, glm::vec3(1.0f, 1.0f, 0.0f));
        renderer->DrawText("WORLD CLASS EDITION", glm::vec2(Width / 2 - 180, Height / 2 - 40), 
            1.0f, glm::vec3(0.0f, 1.0f, 1.0f));
        renderer->DrawText("Press ENTER to Start", glm::vec2(Width / 2 - 140, Height / 2 + 50), 
            1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        renderer->DrawText("HIGH SCORE: " + std::to_string(HighScore), 
            glm::vec2(Width / 2 - 120, Height / 2 + 100), 
            1.0f, glm::vec3(1.0f, 0.5f, 0.0f));
        return;
    }
    
    // Render maze
    maze->Render(renderer.get());
    
    // Render particles (behind entities)
    particles->Render();
    
    // Render player
    player->Render(renderer.get());
    
    // Render ghosts
    for (auto& ghost : ghosts)
    {
        if (ghost->IsActive())
            ghost->Render(renderer.get(), powerUpActive);
    }
    
    // Render UI
    renderer->DrawText("SCORE: " + std::to_string(Score), 
        glm::vec2(20, 30), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    renderer->DrawText("HIGH: " + std::to_string(HighScore), 
        glm::vec2(Width - 200, 30), 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    renderer->DrawText("LEVEL: " + std::to_string(Level), 
        glm::vec2(Width / 2 - 50, 30), 1.0f, glm::vec3(0.0f, 1.0f, 1.0f));
    
    // Render lives
    for (int i = 0; i < Lives; i++)
    {
        renderer->DrawCircle(glm::vec2(20 + i * 30, Height - 30), 10.0f, 
            glm::vec3(1.0f, 1.0f, 0.0f));
    }
    
    if (State == GAME_PAUSED)
    {
        renderer->DrawText("PAUSED", glm::vec2(Width / 2 - 80, Height / 2), 
            2.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else if (State == GAME_OVER)
    {
        renderer->DrawText("GAME OVER", glm::vec2(Width / 2 - 120, Height / 2 - 50), 
            2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        renderer->DrawText("Press ENTER to Continue", glm::vec2(Width / 2 - 150, Height / 2 + 20), 
            1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else if (State == GAME_WIN)
    {
        renderer->DrawText("LEVEL COMPLETE!", glm::vec2(Width / 2 - 150, Height / 2), 
            2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

void Game::Reset()
{
    Score = 0;
    Lives = 3;
    Level = 1;
    powerUpActive = false;
    powerUpTimer = 0.0f;
    ghostReleaseTimer = 0.0f;
    ghostsEaten = 0;
    
    maze->Reset();
    player->Reset(glm::vec2(13.0f, 29.0f));
    
    ghosts[0]->Reset(glm::vec2(13.5f, 11.0f));
    ghosts[1]->Reset(glm::vec2(11.5f, 14.0f));
    ghosts[2]->Reset(glm::vec2(13.5f, 14.0f));
    ghosts[3]->Reset(glm::vec2(15.5f, 14.0f));
    
    ghosts[0]->SetActive(true);
    for (size_t i = 1; i < ghosts.size(); i++)
        ghosts[i]->SetActive(false);
}

void Game::NextLevel()
{
    Level++;
    powerUpActive = false;
    powerUpTimer = 0.0f;
    ghostReleaseTimer = 0.0f;
    ghostsEaten = 0;
    
    maze->Reset();
    player->Reset(glm::vec2(13.0f, 29.0f));
    
    ghosts[0]->Reset(glm::vec2(13.5f, 11.0f));
    ghosts[1]->Reset(glm::vec2(11.5f, 14.0f));
    ghosts[2]->Reset(glm::vec2(13.5f, 14.0f));
    ghosts[3]->Reset(glm::vec2(15.5f, 14.0f));
    
    ghosts[0]->SetActive(true);
    for (size_t i = 1; i < ghosts.size(); i++)
        ghosts[i]->SetActive(false);
    
    stateTimer = 2.0f;
}

void Game::EatDot(int x, int y)
{
    maze->RemoveDot(x, y);
    Score += 10;
    
    if (Score > HighScore)
    {
        HighScore = Score;
        SaveHighScore();
    }
    
    // Spawn particles
    glm::vec2 worldPos = maze->GridToWorld(x, y);
    particles->Spawn(worldPos, glm::vec3(1.0f, 1.0f, 0.5f), 5);
}

void Game::EatPowerPellet()
{
    Score += 50;
    powerUpActive = true;
    powerUpTimer = 8.0f;
    ghostsEaten = 0;
    
    for (auto& ghost : ghosts)
    {
        if (ghost->IsActive() && !ghost->IsEaten())
            ghost->SetFrightened(true);
    }
    
    // Spawn particles
    glm::vec2 playerPos = player->GetPosition();
    particles->Spawn(maze->GridToWorld(playerPos.x, playerPos.y), 
        glm::vec3(1.0f, 0.5f, 1.0f), 20);
}

void Game::EatGhost(Ghost* ghost)
{
    ghostsEaten++;
    int points = 200 * (1 << (ghostsEaten - 1)); // 200, 400, 800, 1600
    Score += points;
    
    if (Score > HighScore)
    {
        HighScore = Score;
        SaveHighScore();
    }
    
    ghost->SetEaten(true);
    
    // Spawn particles
    particles->Spawn(ghost->GetPosition() * 32.0f, 
        glm::vec3(0.5f, 0.5f, 1.0f), 30);
}

void Game::LoseLife()
{
    Lives--;
    
    if (Lives <= 0)
    {
        State = GAME_OVER;
        SaveHighScore();
    }
    else
    {
        // Reset positions
        player->Reset(glm::vec2(13.0f, 29.0f));
        
        ghosts[0]->Reset(glm::vec2(13.5f, 11.0f));
        ghosts[1]->Reset(glm::vec2(11.5f, 14.0f));
        ghosts[2]->Reset(glm::vec2(13.5f, 14.0f));
        ghosts[3]->Reset(glm::vec2(15.5f, 14.0f));
        
        powerUpActive = false;
        powerUpTimer = 0.0f;
        ghostReleaseTimer = 0.0f;
    }
}

bool Game::CheckCollision(const glm::vec2& pos1, const glm::vec2& pos2, float threshold)
{
    return glm::distance(pos1, pos2) < threshold;
}

void Game::LoadHighScore()
{
    std::ifstream file("highscore.dat");
    if (file.is_open())
    {
        file >> HighScore;
        file.close();
    }
}

void Game::SaveHighScore()
{
    std::ofstream file("highscore.dat");
    if (file.is_open())
    {
        file << HighScore;
        file.close();
    }
}
