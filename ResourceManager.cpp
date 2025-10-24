#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, 
                                   const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

void ResourceManager::Clear()
{
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, 
                                          const char* gShaderFile)
{
    std::string vertexCode = loadFileAsString(vShaderFile);
    std::string fragmentCode = loadFileAsString(fShaderFile);
    std::string geometryCode;
    
    if (gShaderFile != nullptr)
        geometryCode = loadFileAsString(gShaderFile);
    
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = gShaderFile != nullptr ? geometryCode.c_str() : nullptr;
    
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderCode);
    return shader;
}

std::string ResourceManager::loadFileAsString(const char* filepath)
{
    std::string content;
    std::ifstream file;
    
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        file.open(filepath);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    }
    catch (std::exception& e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader file: " << filepath << std::endl;
    }
    
    return content;
}
