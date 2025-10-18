#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec3 color;
    float life;
    float size;
    
    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f), size(1.0f) {}
};

class ParticleSystem
{
public:
    ParticleSystem(Shader shader, unsigned int amount);
    
    void Update(float dt);
    void Render();
    void Spawn(glm::vec2 position, glm::vec3 color, int count);
    
private:
    std::vector<Particle> particles;
    unsigned int amount;
    Shader shader;
    unsigned int VAO;
    
    void init();
    unsigned int firstUnusedParticle();
};

#endif
