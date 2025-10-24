#include "ParticleSystem.h"
#include <random>

ParticleSystem::ParticleSystem(Shader shader, unsigned int amount)
    : shader(shader), amount(amount)
{
    init();
}

void ParticleSystem::init()
{
    particles.resize(amount);
    
    // Set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void ParticleSystem::Update(float dt)
{
    for (auto& particle : particles)
    {
        particle.life -= dt;
        
        if (particle.life > 0.0f)
        {
            particle.position -= particle.velocity * dt;
            // Color fades with life (no alpha component in vec3)
        }
    }
}

void ParticleSystem::Render()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for glow effect
    shader.Use();
    
    for (const auto& particle : particles)
    {
        if (particle.life > 0.0f)
        {
            shader.SetVector2f("offset", particle.position);
            shader.SetVector4f("color", glm::vec4(particle.color, particle.life));
            shader.SetFloat("size", particle.size);
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Reset to default blending
}

void ParticleSystem::Spawn(glm::vec2 position, glm::vec3 color, int count)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f);
    static std::uniform_real_distribution<float> randomSpeed(20.0f, 100.0f);
    static std::uniform_real_distribution<float> randomSize(2.0f, 6.0f);
    
    for (int i = 0; i < count; i++)
    {
        unsigned int unusedParticle = firstUnusedParticle();
        
        float angle = glm::radians(randomAngle(gen));
        float speed = randomSpeed(gen);
        
        particles[unusedParticle].position = position;
        particles[unusedParticle].velocity = glm::vec2(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        );
        particles[unusedParticle].color = color;
        particles[unusedParticle].life = 1.0f;
        particles[unusedParticle].size = randomSize(gen);
    }
}

unsigned int ParticleSystem::firstUnusedParticle()
{
    static unsigned int lastUsedParticle = 0;
    
    for (unsigned int i = lastUsedParticle; i < amount; i++)
    {
        if (particles[i].life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }
    
    for (unsigned int i = 0; i < lastUsedParticle; i++)
    {
        if (particles[i].life <= 0.0f)
        {
            lastUsedParticle = i;
            return i;
        }
    }
    
    lastUsedParticle = 0;
    return 0;
}
