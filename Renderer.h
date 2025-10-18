#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <vector>

class Renderer
{
public:
    Renderer(Shader& shader);
    ~Renderer();
    
    void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec3 color, float rotation = 0.0f);
    void DrawRectOutline(glm::vec2 position, glm::vec2 size, glm::vec3 color, float thickness = 2.0f);
    void DrawCircle(glm::vec2 position, float radius, glm::vec3 color);
    void DrawPacman(glm::vec2 position, float radius, float rotation, float mouthAngle, glm::vec3 color);
    void DrawText(const std::string& text, glm::vec2 position, float scale, glm::vec3 color);
    void DrawCharacter(char c, glm::vec2 pos, float width, float height, glm::vec3 color);
    
private:
    Shader shader;
    unsigned int quadVAO;
    unsigned int circleVAO;
    
    void initRenderData();
    void generateCircleVertices(std::vector<float>& vertices, int segments = 32);
    void generatePacmanVertices(std::vector<float>& vertices, float mouthAngle, int segments = 32);
};

#endif
