#ifndef FLOOR_CLASS_H
#define FLOOR_CLASS_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

class Floor
{
public:
    Floor(float x, float y, float z, float width, float length);
    void Draw(Shader& shader);
    VAO VAO1;
    glm::vec3 position;
    glm::mat4 model = glm::mat4(1.0f);
private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    void initVertices(float width, float length);
    void initIndices();
};
#endif
