#include "Renderer.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Renderer::Renderer(Shader& shader)
    : shader(shader)
{
    initRenderData();
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteVertexArrays(1, &circleVAO);
}

void Renderer::initRenderData()
{
    // Configure quad VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Configure circle VAO
    glGenVertexArrays(1, &circleVAO);
}

void Renderer::DrawRect(glm::vec2 position, glm::vec2 size, glm::vec3 color, float rotation)
{
    shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    
    shader.SetMatrix4("model", model);
    shader.SetVector3f("spriteColor", color);
    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::DrawRectOutline(glm::vec2 position, glm::vec2 size, glm::vec3 color, float thickness)
{
    // Draw four lines for the outline
    DrawRect(position, glm::vec2(size.x, thickness), color); // Top
    DrawRect(position + glm::vec2(0, size.y - thickness), glm::vec2(size.x, thickness), color); // Bottom
    DrawRect(position, glm::vec2(thickness, size.y), color); // Left
    DrawRect(position + glm::vec2(size.x - thickness, 0), glm::vec2(thickness, size.y), color); // Right
}

void Renderer::DrawCircle(glm::vec2 position, float radius, glm::vec3 color)
{
    std::vector<float> vertices;
    generateCircleVertices(vertices, 32);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(circleVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f));
    
    shader.SetMatrix4("model", model);
    shader.SetVector3f("spriteColor", color);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

void Renderer::DrawPacman(glm::vec2 position, float radius, float rotation, float mouthAngle, glm::vec3 color)
{
    std::vector<float> vertices;
    generatePacmanVertices(vertices, mouthAngle, 32);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray(circleVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
    shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f));
    
    shader.SetMatrix4("model", model);
    shader.SetVector3f("spriteColor", color);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

void Renderer::DrawText(const std::string& text, glm::vec2 position, float scale, glm::vec3 color)
{
    // Create proper bitmap font rendering
    float charWidth = 12.0f * scale;
    float charHeight = 16.0f * scale;
    
    for (size_t i = 0; i < text.length(); i++)
    {
        char c = text[i];
        glm::vec2 charPos = position + glm::vec2(i * charWidth, 0.0f);
        
        if (c == ' ') continue;
        
        // Draw character based on ASCII value with proper shapes
        DrawCharacter(c, charPos, charWidth, charHeight, color);
    }
}

void Renderer::DrawCharacter(char c, glm::vec2 pos, float width, float height, glm::vec3 color)
{
    float w = width * 0.8f;
    float h = height * 0.8f;
    
    // Draw different shapes for different characters
    switch (c) {
        case 'P':
            // Draw P shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w * 0.7f, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w * 0.6f, h * 0.2f), color); // Middle line
            DrawRect(pos + glm::vec2(w * 0.5f, 0), glm::vec2(w * 0.2f, h * 0.6f), color); // Right line (top half)
            break;
        case 'A':
            // Draw A shape
            DrawRect(pos + glm::vec2(w * 0.4f, 0), glm::vec2(w * 0.2f, h), color); // Center line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos, glm::vec2(w * 0.2f, h * 0.8f), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h * 0.8f), color); // Right line
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w, h * 0.2f), color); // Middle line
            break;
        case 'C':
            // Draw C shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'M':
            // Draw M shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right line
            DrawRect(pos + glm::vec2(w * 0.3f, h * 0.5f), glm::vec2(w * 0.15f, h * 0.5f), color); // Left diagonal
            DrawRect(pos + glm::vec2(w * 0.55f, h * 0.5f), glm::vec2(w * 0.15f, h * 0.5f), color); // Right diagonal
            break;
        case 'N':
            // Draw N shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right line
            DrawRect(pos + glm::vec2(w * 0.2f, h * 0.4f), glm::vec2(w * 0.6f, h * 0.2f), color); // Diagonal
            break;
        case '-':
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w, h * 0.2f), color);
            break;
        case 'E':
            // Draw E shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w * 0.8f, h * 0.2f), color); // Middle line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'T':
            // Draw T shape
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(w * 0.4f, 0), glm::vec2(w * 0.2f, h), color); // Center line
            break;
        case 'R':
            // Draw R shape
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w * 0.7f, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w * 0.6f, h * 0.2f), color); // Middle line
            DrawRect(pos + glm::vec2(w * 0.5f, 0), glm::vec2(w * 0.2f, h * 0.6f), color); // Right line (top half)
            DrawRect(pos + glm::vec2(w * 0.3f, h * 0.6f), glm::vec2(w * 0.5f, h * 0.4f), color); // Diagonal
            break;
        case 'S':
            // Draw S shape
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos, glm::vec2(w * 0.2f, h * 0.4f), color); // Top left
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w, h * 0.2f), color); // Middle line
            DrawRect(pos + glm::vec2(w * 0.8f, h * 0.6f), glm::vec2(w * 0.2f, h * 0.4f), color); // Bottom right
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case ':':
            DrawRect(pos + glm::vec2(w * 0.4f, h * 0.2f), glm::vec2(w * 0.2f, h * 0.2f), color);
            DrawRect(pos + glm::vec2(w * 0.4f, h * 0.6f), glm::vec2(w * 0.2f, h * 0.2f), color);
            break;
        case 'L':
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'D':
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w * 0.7f, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w * 0.7f, h * 0.2f), color); // Bottom line
            DrawRect(pos + glm::vec2(w * 0.7f, h * 0.2f), glm::vec2(w * 0.2f, h * 0.6f), color); // Right line
            break;
        case 'I':
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(w * 0.4f, 0), glm::vec2(w * 0.2f, h), color); // Center line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'O':
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right
            break;
        case 'W':
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right line
            DrawRect(pos + glm::vec2(w * 0.4f, h * 0.5f), glm::vec2(w * 0.2f, h * 0.5f), color); // Center line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'G':
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            DrawRect(pos + glm::vec2(w * 0.5f, h * 0.4f), glm::vec2(w * 0.5f, h * 0.2f), color); // Middle right
            DrawRect(pos + glm::vec2(w * 0.8f, h * 0.4f), glm::vec2(w * 0.2f, h * 0.4f), color); // Right bottom
            break;
        case 'H':
            DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right line
            DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w, h * 0.2f), color); // Middle line
            break;
        case 'U':
            DrawRect(pos, glm::vec2(w * 0.2f, h * 0.8f), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h * 0.8f), color); // Right line
            DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom line
            break;
        case 'V':
            DrawRect(pos, glm::vec2(w * 0.2f, h * 0.7f), color); // Left line
            DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h * 0.7f), color); // Right line
            DrawRect(pos + glm::vec2(w * 0.2f, h * 0.7f), glm::vec2(w * 0.6f, h * 0.3f), color); // Bottom diagonal
            break;
        default:
            if (c >= '0' && c <= '9') {
                // Draw numbers as rectangles with internal patterns
                DrawRect(pos, glm::vec2(w, h * 0.2f), color); // Top
                DrawRect(pos + glm::vec2(0, h * 0.8f), glm::vec2(w, h * 0.2f), color); // Bottom
                DrawRect(pos, glm::vec2(w * 0.2f, h), color); // Left
                DrawRect(pos + glm::vec2(w * 0.8f, 0), glm::vec2(w * 0.2f, h), color); // Right
                if (c != '0') {
                    DrawRect(pos + glm::vec2(0, h * 0.4f), glm::vec2(w, h * 0.2f), color); // Middle for non-zero
                }
            } else {
                // Default: draw as filled rectangle
                DrawRect(pos, glm::vec2(w, h), color);
            }
            break;
    }
}

void Renderer::generateCircleVertices(std::vector<float>& vertices, int segments)
{
    vertices.clear();
    vertices.push_back(0.0f); // Center x
    vertices.push_back(0.0f); // Center y
    
    for (int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        vertices.push_back(std::cos(angle));
        vertices.push_back(std::sin(angle));
    }
}

void Renderer::generatePacmanVertices(std::vector<float>& vertices, float mouthAngle, int segments)
{
    vertices.clear();
    vertices.push_back(0.0f); // Center x
    vertices.push_back(0.0f); // Center y
    
    float startAngle = glm::radians(mouthAngle);
    float endAngle = 2.0f * M_PI - glm::radians(mouthAngle);
    
    for (int i = 0; i <= segments; i++)
    {
        float t = (float)i / segments;
        float angle = startAngle + t * (endAngle - startAngle);
        vertices.push_back(std::cos(angle));
        vertices.push_back(std::sin(angle));
    }
}
