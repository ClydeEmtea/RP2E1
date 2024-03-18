#include "Floor.h"

Floor::Floor(float x, float y, float z, float width, float length)
{
    this->position = glm::vec3(x, y, z);
    initVertices(width, length);
    initIndices();

    VAO1.Bind();
    VBO VBO1(vertices.data(), sizeof(GLfloat) * vertices.size());
    EBO EBO1(indices.data(), sizeof(GLuint) * indices.size());

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

void Floor::Draw(Shader& shader)
{
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Draw the floor
    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO1.Unbind();
}

void Floor::initVertices(float width, float length)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    vertices =          
    {   //  Position                   Color                 Texcoords          Normal
        x, y, z,                    0.0f, 0.0f, 0.0f,    0.0f, 0.0f,       0.0f, 1.0f, 0.0f, // Bottom left
        x + width, y, z,            0.0f, 0.0f, 0.0f,    1.0f, 0.0f,       0.0f, 1.0f, 0.0f, // Bottom right
        x + width, y, z - length,   0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top right
        x, y, z - length,           0.0f, 0.0f, 0.0f,    0.0f, 1.0f,       0.0f, 1.0f, 0.0f  // Top left
    };
}

void Floor::initIndices()
{
    indices =
    {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };
}
