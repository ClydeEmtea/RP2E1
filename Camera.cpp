#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position, std::vector<Cube> cubes)
{
	Camera::width = width;
	Camera::height = height;
	Camera::Position = position;
	Camera::cubes = cubes;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}



void Camera::Inputs(GLFWwindow* window)
{
	glm::vec3 movement(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movement += glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 left = glm::normalize(glm::cross(Orientation, Up));
		movement -= left;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		movement -= glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
		movement += right;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		spacePressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && spacePressed)
	{
		if (!viewFar)
		{
			goingUp = true;
			viewFar = true;
			goingDown = false;
			verticalSpeed = 0.001f;
		}
		else if (viewFar)
		{
			goingDown = true;
			viewFar = false;
			goingUp = false;
			verticalSpeed = 0.001f;
		}
		spacePressed = false;
	}


	if (goingUp)
	{
		Position += verticalSpeed * Up;
		verticalSpeed += 0.002f;
		nearPlane -= 0.0015f;
		if (Position.y >= 3.0f)
		{
			goingUp = false;
			verticalSpeed = 0.001f;
			nearPlane = 0.001f;
		}
	}
	if (goingDown)
	{
		Position -= verticalSpeed * Up;
		verticalSpeed += 0.002f;
		nearPlane += 0.0015f;
		if (Position.y <= -1.0f)
		{
			goingDown = false;
			verticalSpeed = 0.001f;
			nearPlane = 0.1f;
		}
	}

	// Normalize the movement vector if it's not a zero vector
	if (glm::length(movement) > 0.0f)
	{
		movement = glm::normalize(movement);
	}

	// Check for collisions with cubes
	for (const auto& cube : cubes) {
		// Get the bounding box of the cube

		if ((Position.x >= cube.position.x + 0.3f && Position.x <= cube.position.x + 1.7f) &&
			(Position.z >= cube.position.z - 0.7f && Position.z <= cube.position.z + 0.7f)) {
			// Define small offsets to try moving around the obstacle
			float offset = 0.01f;

			while ((Position.x >= cube.position.x + 0.3f && Position.x <= cube.position.x + 1.7f) &&
				(Position.z >= cube.position.z - 0.7f && Position.z <= cube.position.z + 0.7f))
			{
				for (int i = 0; i < 4; i++)
				{
					switch (i)
					{
						case 0:	Position.x += offset; break;
						case 1: Position.x -= offset; break;
						case 2: Position.z += offset; break;
						case 3: Position.z -= offset; break;
					}
					if (!((Position.x >= cube.position.x + 0.3f && Position.x <= cube.position.x + 1.7f) &&
						(Position.z >= cube.position.z - 0.7f && Position.z <= cube.position.z + 0.7f))) break;
					switch (i)
					{
						case 0:	Position.x -= offset; break;
						case 1: Position.x += offset; break;
						case 2: Position.z -= offset; break;
						case 3: Position.z += offset; break;
					}
				}
				offset += 0.01f;
			}
		}
	}


	// Update position with the normalized movement vector
	if (!viewFar && !goingDown && !goingUp)
	Position += speed * movement;



	// Mouse input

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;
	// Fetches the coordinates of the cursor
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
	// and then "transforms" them into degrees 
	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// Calculates upcoming vertical change in the Orientation
	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	// Decides whether or not the next vertical Orientation is legal or not
	if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		Orientation = newOrientation;
	}

	// Rotates the Orientation left and right
	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
	glfwSetCursorPos(window, (width / 2), (height / 2));
}

