#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

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

class Cube
{
public:
	Cube(float x, float y, float z);
	void Draw(Shader& shader);
	VAO VAO1;
	glm::vec3 position;
	glm::mat4 model = glm::mat4(1.0f);
	int texNum;
private:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
		


	void initVertices();
	void initIndices();
};
#endif