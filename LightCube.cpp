#include "LightCube.h"

LightCube::LightCube(glm::vec3 position)
{
    this->position = position;
    initVertices();
    initIndices();

    VAO1.Bind();
    VBO VBO1(vertices.data(), sizeof(GLfloat) * vertices.size());
    EBO EBO1(indices.data(), sizeof(GLuint) * indices.size());

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

void LightCube::Draw(Shader& shader)
{
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO1.Unbind();
}

void LightCube::initVertices()
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    vertices =
    {
        x - 0.1f, y + 0.1f, z + 0.1f, // 0
        x - 0.1f, y + 0.1f, z - 0.1f, // 1
        x + 0.1f, y + 0.1f, z - 0.1f, // 2
        x + 0.1f, y + 0.1f, z + 0.1f, // 3
        x - 0.1f, y - 0.1f, z + 0.1f, // 4
        x - 0.1f, y - 0.1f, z - 0.1f, // 5
        x + 0.1f, y - 0.1f, z - 0.1f, // 6
        x + 0.1f, y - 0.1f, z + 0.1f  // 7
    };
}

void LightCube::initIndices()
{
    indices =
    {
        0, 1, 2,  // Top face
        2, 3, 0,
        4, 5, 6,  // Bottom face
        6, 7, 4,
        0, 4, 7,  // Front face
        7, 3, 0,
        3, 7, 6,  // Right face
        6, 2, 3,
        2, 6, 5,  // Back face
        5, 1, 2,
        1, 5, 4,  // Left face
        4, 0, 1
    };
}
