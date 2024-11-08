#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Cube.h"
#include"LightCube.h"
#include "Map.h"
#include"Floor.h"


const unsigned int width = 1200;
const unsigned int height = 800;

std::vector<Cube> cubes;
std::vector<LightCube> lightCubes;

enum class GameState
{
	MainMenu, Game, GameOver
};

GameState gameState = GameState::Game;

void renderMainMenu(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		gameState = GameState::Game;
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	int mapLength = 40;
	int mapWidth = 40;
	Map maze(mapLength, mapWidth);

	Floor floor(0.5f, -1.5f, mapWidth - 0.5f, mapWidth, mapLength);

	// Generates Vertex Array Object and binds it
	for (int i = 0; i < mapLength; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (maze.map[i][j] == 1)
			{
				cubes.emplace_back(j, 0, i);
			}
			if (maze.map[i][j] == 2)
			{
				lightCubes.emplace_back((glm::vec3(j, -2.0f, i)));
			}
			if (i == 0 || j == 0)
			{
				cubes.emplace_back(j, 0, i);
			}
			if (j == 0 || j == mapWidth - 1 || i == 0 || i == mapLength - 1)
			{
				cubes.emplace_back(j, 1, i);
			}
		}
	}
	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");

	Camera camera(width, height, glm::vec3(3.0f, -1.0f, 2.0f), cubes);


	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = camera.Position;
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = cubes[0].position;
	glm::mat4 objectModel = cubes[0].model;
	objectModel = glm::translate(objectModel, objectPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);

	Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 1);



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Creates camera object

	glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), camera.nearPlane);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.01f, 0.01f, 0.02f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (gameState == GameState::MainMenu)
		{
			renderMainMenu(window);
		}
		else if (gameState == GameState::Game)
		{
			// Handles camera inputs
			camera.Inputs(window);
			// Updates and exports the camera matrix to the Vertex Shader
			camera.updateMatrix(45.0f, 0.1f, 100.0f);
			lightPos = camera.Position;
			lightPos.y += 1.5f;

			// Tells OpenGL which Shader Program we want to use
			shaderProgram.Activate();
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

			// Exports the camera Position to the Fragment Shader for specular lighting
			glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

			glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), camera.nearPlane);

			// Export the camMatrix to the Vertex Shader of the pyramid
			camera.Matrix(shaderProgram, "camMatrix");

			// Bind the VAO so OpenGL knows to use it
			brickTex.Bind();

			floor.Draw(shaderProgram);

			for (int i = 0; i < cubes.size(); i++)
			{
				if (cubes[i].texNum != 1)
				{
					brickTex.Bind();
				}
				else
				{
					popCat.Bind();
				}
				cubes[i].Draw(shaderProgram);
			}

			if ((camera.Position.x >= lightCubes[0].position.x - 0.5f && camera.Position.x <= lightCubes[0].position.x + 0.5f) &&
				(camera.Position.z >= lightCubes[0].position.z - 0.5f && camera.Position.z <= lightCubes[0].position.z + 0.5f))
			{
				gameState = GameState::GameOver;
			}


			// Tells OpenGL which Shader Program we want to use
			lightShader.Activate();
			// Export the camMatrix to the Vertex Shader of the light cube
			camera.Matrix(lightShader, "camMatrix");
			// Bind the VAO so OpenGL knows to use it
			lightCubes[0].Draw(lightShader);
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	for (int i = 0; i < cubes.size(); i++)
	{
		cubes[i].VAO1.Delete();
	}
	brickTex.Delete();
	shaderProgram.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}