#ifndef PLANET_H
#define PLANET_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"shaderClass.h"


class Planet
{
public:
    Planet(float radius, int segments);
    

    void Draw();
    void ModelTranslate(glm::vec4 Color, glm::vec3 Position, glm::mat4 Model, GLfloat radius, GLfloat speed, GLfloat spinSpeed);
    void ShaderConfigure(Shader planetShader);
    void ShaderConfigureWithSunReflection(Shader planetShader, Planet sun);
    void Delete();
    void movePlanet();
    void Draw(Shader planetShader);
    void move();
   

    glm::vec4 planetColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 planetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 planetModel = glm::mat4(1.0f);

    GLfloat* vertices;
    GLuint* indices;
    GLfloat orbitRadius;
    GLfloat orbitSpeed;
    GLfloat rotationSpeed;
    unsigned int sizeofVertices;
    unsigned int sizeofIndices;

    VAO planetVAO;
    VBO* planetVBO;
    EBO* planetEBO;


    float radius;
    int segments;

    void GenerateVertices();
    void GenerateIndices();
};

#endif