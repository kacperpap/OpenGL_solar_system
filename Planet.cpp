#include "Planet.h"
#include <glm/gtc/constants.hpp>

#include"Texture.h"

#include"Camera.h"

Planet::Planet(float radius, int segments) :
    radius(radius), segments(segments)
{
    GenerateVertices();
    GenerateIndices();

    this->planetVAO.Bind();

    // uwaga: nie przypisuj sizeof(this->vertices), bo wartoœæ do bêdzie tylko wielkoœæ wskaŸnika -> trzeba obliczyæ recznie
    this->planetVBO = new VBO(this->vertices, this->sizeofVertices * sizeof(GLfloat)); 
    this->planetEBO = new EBO(this->indices, this->sizeofIndices * sizeof(GLuint));

    this->planetVAO.LinkAttrib(*this->planetVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    this->planetVAO.LinkAttrib(*this->planetVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); //id of texCoor = 1
    this->planetVAO.LinkAttrib(*this->planetVBO, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    this->planetVAO.Unbind();
    (*this->planetVBO).Unbind();
    (*this->planetEBO).Unbind();
}

void Planet::GenerateVertices()
{
    this->sizeofVertices = 8 * (this->segments + 1) * (this->segments + 1);
    this->vertices = new GLfloat[this->sizeofVertices];

    for (int j = 0; j <= this->segments; j++) {
        float theta = j * glm::pi<float>() / this->segments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int i = 0; i <= this->segments; i++) {
            float phi = i * 2 * glm::pi<float>() / this->segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            glm::vec3 normal(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
            glm::vec2 texCoord(i / (float)this->segments, j / (float)this->segments);
            glm::vec3 position = this->radius * normal;

            vertices[(j * (this->segments + 1) + i) * 8 + 0] = position.x;
            vertices[(j * (this->segments + 1) + i) * 8 + 1] = position.y;
            vertices[(j * (this->segments + 1) + i) * 8 + 2] = position.z;
            vertices[(j * (this->segments + 1) + i) * 8 + 3] = texCoord.x;
            vertices[(j * (this->segments + 1) + i) * 8 + 4] = texCoord.y;
            vertices[(j * (this->segments + 1) + i) * 8 + 5] = normal.x;
            vertices[(j * (this->segments + 1) + i) * 8 + 6] = normal.y;
            vertices[(j * (this->segments + 1) + i) * 8 + 7] = normal.z;
        }
    }
}

void Planet::GenerateIndices()
{
    this->sizeofIndices = 6 * this->segments * this->segments;
    this->indices = new GLuint[sizeofIndices];

    for (int j = 0; j < this->segments; j++) {
        for (int i = 0; i < this->segments; i++) {
            int p1 = j * (this->segments + 1) + i;
            int p2 = p1 + (this->segments + 1);

            indices[(j * this->segments + i) * 6 + 0] = p1;
            indices[(j * this->segments + i) * 6 + 1] = p2;
            indices[(j * this->segments + i) * 6 + 2] = p1 + 1;
            indices[(j * this->segments + i) * 6 + 3] = p1 + 1;
            indices[(j * this->segments + i) * 6 + 4] = p2;
            indices[(j * this->segments + i) * 6 + 5] = p2 + 1;
        }
    }
}


void Planet::ModelTranslate(glm::vec4 Color, glm::vec3 Position, glm::mat4 Model, GLfloat radius, GLfloat speed, GLfloat spinSpeed)
{
    this->orbitRadius = radius;
    this->orbitSpeed = speed;
    this->rotationSpeed = spinSpeed;
    this->planetColor = Color;
    this->planetPos = Position;
    this->planetModel = Model;
    this->planetModel = glm::translate(this->planetModel, this->planetPos);
}


//Wywo³anie musi byæ po ModelTranslate, tak aby przypisaæ dobre wartosci pol
void Planet::ShaderConfigure(Shader planetShader) 
{
    planetShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(this->planetModel));
    //glUniform4f(glGetUniformLocation(planetShader.ID, "lightColor"), this->planetColor.x, this->planetColor.y, this->planetColor.z, this->planetColor.w);

}

void Planet::ShaderConfigureWithSunReflection(Shader planetShader, Planet sun)
{
    planetShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(this->planetModel));
    glUniform4f(glGetUniformLocation(planetShader.ID, "lightColor"), sun.planetColor.x, sun.planetColor.y, sun.planetColor.z, sun.planetColor.w);
    glUniform3f(glGetUniformLocation(planetShader.ID, "lightPos"), sun.planetPos.x, sun.planetPos.y, sun.planetPos.z);
}


void Planet::move()
{
    //tutaj obracaja siê wokol siebie ale wszystkie tak samo idk czemu
    //static GLfloat spin_angle = 0.0f;
    //static GLfloat rot_angle = 0.0f;;
    //rot_angle += glm::radians(this->orbitSpeed) / 200.0f;
    //spin_angle += glm::radians(this->rotationSpeed) / 100.0f;


    this->speed += glm::radians(this->orbitSpeed) / 200.0f;
    this->spin += glm::radians(this->rotationSpeed) / 100.0f;

    glm::vec3 tvec = glm::vec3(this->orbitRadius, 0.0f, 0.0f);
    glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f); //OY

    glm::mat4 translate_ = glm::translate(glm::mat4(1.0f), -tvec);
    glm::mat4 rotate_ = glm::rotate(glm::mat4(1.0f), this->speed, axis);
    glm::mat4 spin_ = glm::rotate(glm::mat4(1.0f), this->spin, axis);
    this->planetModel = rotate_ * translate_ * spin_;
}

void Planet:: movePlanet()
{
    // tu sie obracalo dobrze, ale teraz nie dziala ktos mi powie czemu???
    //this->planetModel = glm::translate(this->planetModel, glm::vec3( -this->orbitRadius  , 0.0f, 0.0f));
    //this->planetModel = glm::rotate(this->planetModel, glm::radians(this->orbitSpeed) / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //OY
    //this->planetModel = glm::translate(this->planetModel,glm::vec3(this->orbitRadius, 0.0f, 0.0f));
}


void Planet::Draw(Shader planetShader)
{
    glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(this->planetModel));
    this->planetVAO.Bind();
    glDrawElements(GL_TRIANGLES, 6 * this->segments * this->segments, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Planet::Delete()
{
    this->planetVAO.Delete();
    this->planetVBO->Delete();
    this->planetEBO->Delete();
}
