// Code adapted from www.learnopengl.com, www.glfw.org

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

// Window dimensions
const GLuint WIDTH = 640, HEIGHT = 640;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current

	glfwSetKeyCallback(window, key_callback);// Set the required callback functions

	glfwSetCursorPosCallback(window, mouse_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, HEIGHT, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
	GLfloat vertices[] = {
		//firstTriangle
		 0.4f,  0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Top Right 
		 0.4f, -0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Bottom Right 
		 0.1f, -0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Bottom Left 

		 0.4f,  0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Top Right 
		 0.1f, 0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Bottom Right 
		 0.1f, -0.08f, 0.0f, 0.1f, 0.0f, 1.0f,  // Bottom Left
		 //secondTriangle
		 -0.1f,  0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.1f, -0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.4f, -0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 

		 -0.1f,  0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.4f, 0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.4f, -0.08f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 
		 //thirdTriangle
		 0.08f, 0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 0.08f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.08f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 

		 0.08f, 0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.08f, 0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.08f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 
		 //fourthTriangle
		 -0.08f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.08f, -0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 0.08f, -0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 

		 -0.08f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 0.08f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 0.08f, -0.4f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 
		 //firstMiddleTriangle
		 0.1f, 0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 0.1f, -0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.1f, -0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 

		 0.1f, 0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.1f, 0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.1f, -0.025f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left 
		 //secondMiddleTriangle
		 0.025f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 0.025f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.025f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left

		 0.025f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Top Right 
		 -0.025f, 0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Right 
		 -0.025f, -0.1f, 0.0f, 0.6f, 0.0f, 1.0f,  // Bottom Left

		 //spinningMiddleTriangle1
		 -0.025f, 0.025f, 0.0f, 0.0f, 0.0f, 1.0f,
		 -0.025f, 0.025f, -0.3f, 0.0f, 0.0f, 1.0f,
		 -0.025f, -0.025f, -0.3f, 0.0f, 0.0f, 1.0f,

		 -0.025f, -0.025f, 0.0f, 1.0f, 0.0f, 1.0f,
		 -0.025f, 0.025f, 0.0f, 1.0f, 0.0f, 1.0f,
		 -0.025f, -0.025f, -0.3f, 1.0f, 0.0f, 1.0f,

		 //spinningMiddleTriangle2
		 0.025f, 0.025f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.025f, 0.025f, -0.3f, 1.0f, 0.0f, 1.0f,
		 0.025f, -0.025f, -0.3f, 1.0f, 0.0f, 1.0f,

		 0.025f, -0.025f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.025f, 0.025f, 0.0f, 0.0f, 1.0f, 1.0f,
		 0.025f, -0.025f, -0.3f, 0.0f, 1.0f, 1.0f,

		 //spinningMiddleTriangle3
		 -0.025f, 0.025f, -0.3f, 1.0f, 0.0f, 1.0f,
		 -0.025f, 0.025f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.025f, 0.025f, 0.0f, 1.0f, 0.0f, 1.0f,

		 0.025f, 0.025f, -0.3f, 1.0f, 1.0f, 1.0f,
		 0.025f, 0.025f, 0.0f, 1.0f, 1.0f, 1.0f,
		 -0.025f, 0.025f, -0.3f, 1.0f, 1.0f, 1.0f,

		 //spinningMiddleTriangle4
		 -0.025f, -0.025f, -0.3f, 1.0f, 0.0f, 1.0f,
		 -0.025f, -0.025f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.025f, -0.025f, 0.0f, 1.0f, 0.0f, 1.0f,

		 0.025f, -0.025f, -0.3f, 1.0f, 1.0f, 1.0f,
		 0.025f, -0.025f, 0.0f, 1.0f, 1.0f, 1.0f,
		 -0.025f, -0.025f, -0.3f, 1.0f, 1.0f, 1.0f,
	};

	GLfloat nextShape[] = {
		//frontTriangle1
		 0.15f,  0.15f, -0.3f, 0.1f, 0.0f, 1.0f,
	     0.15f, -0.5f, -0.3f, 0.1f, 0.0f, 1.0f,
	     -0.15f, -0.5f, -0.3f, 0.1f, 0.0f, 1.0f,
		 //frontTriangle2
		 0.15f,  0.15f, -0.3f, 0.5f, 0.0f, 1.0f,
		 -0.15f, 0.15f, -0.3f, 0.5f, 0.0f, 1.0f,
		 -0.15f, -0.5f, -0.3f, 0.5f, 0.0f, 1.0f,

		 //backTriangle1
		 0.15f,  0.15f, -1.2f, 0.1f, 0.0f, 1.0f,
		 0.15f, -0.5f, -1.2f, 0.1f, 0.0f, 1.0f,
		 -0.15f, -0.5f, -1.2f, 0.1f, 0.0f, 1.0f,
		 //backTriangle2
		 0.15f,  0.15f, -1.2f, 0.5f, 0.0f, 1.0f,
		 -0.15f, 0.15f, -1.2f, 0.5f, 0.0f, 1.0f,
		 -0.15f, -0.5f, -1.2f, 0.5f, 0.0f, 1.0f,

		 //sideTriangle1
		 -0.7f, 0.15f, -0.6f, 0.0f, 0.0f, 1.0f,
         -0.7f, 0.15f, -0.9f, 0.0f, 0.0f, 1.0f,
         -0.7f, -0.5f, -0.9f, 0.0f, 0.0f, 1.0f,

		 -0.7f, 0.15f, -0.6f, 0.0f, 0.0f, 1.0f,
		 -0.7f, -0.5f, -0.6f, 0.0f, 0.0f, 1.0f,
		 -0.7f, -0.5f, -0.9f, 0.0f, 0.0f, 1.0f,

	};


	GLuint VBOs[10], VAOs[10];
	glGenVertexArrays(10, VAOs); // the address-of operator (&) is not needed here, as the array name is a pointer type 
	glGenBuffers(10, VBOs);

	// ================================
	// buffer setup
	// ===============================
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nextShape), nextShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		do_movement();

		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the cube
		// use shader
		glUseProgram(shaderProgram);

		// first shape
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// draw object
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 20);
		glBindVertexArray(0);


		// next shape
		glUseProgram(shaderProgram);
		GLint transformLoc;
		glm::mat4 model1;
		transformLoc = glGetUniformLocation(shaderProgram, "model");
		float startTime2 = (float)glfwGetTime();
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 10);


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}

//Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}



void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 0.5f * deltaTime;
	//moveForward
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	//moveBackward
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	//moveLeft
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//moveRight
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//moveUp
	if (keys[GLFW_KEY_I])
		cameraPos += cameraSpeed * cameraUp;
	//moveDown
	if (keys[GLFW_KEY_K])
		cameraPos -= cameraSpeed * cameraUp;
	//rotateRight
	if (keys[GLFW_KEY_L]) {
		float yawAngle = -25.0f * deltaTime;
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yawAngle), cameraUp);
		cameraFront = glm::mat3(rotationMatrix) * cameraFront;
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	//rotateLeft
	if (keys[GLFW_KEY_J]) {
		float yawAngle = 25.0f * deltaTime;
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yawAngle), cameraUp);
		cameraFront = glm::mat3(rotationMatrix) * cameraFront;
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}



}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = (GLfloat)ypos - lastY;
	//GLfloat yoffset = lastY - (GLfloat) ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	GLfloat sensitivity = (GLfloat)0.00025;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}