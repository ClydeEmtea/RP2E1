#include "Cube.h"

Cube::Cube(float x, float y, float z)
{
	this->position = glm::vec3(x, y, z);
	// get random number between 0 and 3
	this->texNum = rand() % 4;
	initVertices();
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

void Cube::Draw(Shader& shader)
{
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Draw the cube
	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	VAO1.Unbind();
}

void Cube::initVertices()
{
	float x = position.x;
	float y = position.y;
	float z = position.z;

	vertices =
	{	// 	Position						Color			Texcoords		Normal
		// Front face
		x + 0.5f, y - 1.5f, z + 0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 0
		x + 1.5f, y - 1.5f, z + 0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 1
		x + 1.5f, y - 0.5f, z + 0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 2
		x + 0.5f, y - 0.5f, z + 0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 3

		// Back face
		x + 0.5f, y - 1.5f, z - 0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 4
		x + 1.5f, y - 1.5f, z - 0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 5
		x + 1.5f, y - 0.5f, z - 0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 6
		x + 0.5f, y - 0.5f, z - 0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 7

		// Right face
		x + 1.5f, y - 1.5f, z + 0.5f,    1.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 8
		x + 1.5f, y - 1.5f, z - 0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 9
		x + 1.5f, y - 0.5f, z - 0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 10
		x + 1.5f, y - 0.5f, z + 0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 11

		// Left face
		x + 0.5f, y - 1.5f, z + 0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 12
		x + 0.5f, y - 1.5f, z - 0.5f,    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 13
		x + 0.5f, y - 0.5f, z - 0.5f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 14
		x + 0.5f, y - 0.5f, z + 0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 15

		// Top face
		x + 0.5f, y - 0.5f, z + 0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 16
		x + 1.5f, y - 0.5f, z + 0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 17
		x + 1.5f, y - 0.5f, z - 0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 18
		x + 0.5f, y - 0.5f, z - 0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 19

		// Bottom face
		x + 0.5f, y - 1.5f, z + 0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 20
		x + 1.5f, y - 1.5f, z + 0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // 21
		x + 1.5f, y - 1.5f, z - 0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,      0.0f, 1.0f, 0.0f, // 22
		x + 0.5f, y - 1.5f, z - 0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 1.0f,      0.0f, 1.0f, 0.0f  // 23
	};
}


void Cube::initIndices()
{
	indices =
	{
			0, 1, 2,  // Front face
				2, 3, 0,
			4, 5, 6,  // Back face
				6, 7, 4,
			8, 9, 10,  // Right face
				10, 11, 8,
			12, 13, 14,  // Left face
				14, 15, 12,
			16, 17, 18,  // Top face
				18, 19, 16,
			20, 21, 22,  // Bottom face
				22, 23, 20
		};
}
