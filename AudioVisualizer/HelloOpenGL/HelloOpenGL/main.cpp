#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/GLU.h>

#include "camera.h"
#include "matrix.h"
#include "vector.h"

#include "triangle_demo.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void onWindowResized(GLFWwindow* window, int width, int height)
{
	if (height == 0) height = 1;						// Prevent A Divide By Zero By making Height Equal One

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

// Simple camera controller. (MOUSE)
Camera gCamera;
void onMouseMove(GLFWwindow* window, double x, double y)
{
	static int lastX = -1, lastY = -1;
	if (lastX == -1 && lastY == -1)
	{
		lastX = x;
		lastY = y;
		return;
	}

	int offsetX = x - lastX;
	int offsetY = y - lastY;
	lastX = x; lastY = y;

	gCamera.rotate(offsetX * 0.1f, Vector(0.0f, 1.0f, 0.0f));
	gCamera.rotateLocal(offsetY * 0.1f, Vector(1.0f, 0.0f, 0.0f));
}

GLFWwindow* window;
int main()
{
	int running = GL_TRUE;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

	// Open an OpenGL window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

	// Hook window resize.
	glfwSetWindowSizeCallback(window, onWindowResized);
	   
	/* Make the window's context current */
    glfwMakeContextCurrent(window);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// hook mouse move callback and lock/hide mouse cursor.
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	// initialize OpenGL.
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Initialize camera.
	gCamera.translate(0.0f, 1.5f, 28.0f);
	gCamera.rotate(-5, Vector(1.0f, 0.0f, 0.0f));
	Matrix matrix(Matrix::makeIdentityMatrix());
	Matrix rotateMatrix(Matrix::makeRotateMatrix(0.5f, Vector(0.0f, 1.0f, 0.0f)));

	// Initialize demo.
	DemoBase* demo = new TriangleDemo();
	demo->init();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// OpenGL rendering goes here...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Simple camera controller. (KEYBOARD)
		float camMoveOffsetX = 0.0f, camMoveOffsetY = 0.0f, camMoveOffsetZ = 0.0f;
		if (glfwGetKey(window, 'A')) camMoveOffsetX -= 0.05f;
		if (glfwGetKey(window, 'D')) camMoveOffsetX += 0.05f;
		if (glfwGetKey(window, 'W')) camMoveOffsetZ -= 0.05f;
		if (glfwGetKey(window, 'S')) camMoveOffsetZ += 0.05f;
		if (glfwGetKey(window, 'Q')) camMoveOffsetY -= 0.05f;
		if (glfwGetKey(window, 'E')) camMoveOffsetY += 0.05f;
		gCamera.translateLocal(camMoveOffsetX, camMoveOffsetY, camMoveOffsetZ);

		// Check if ESC key was pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		demo->draw(gCamera.getViewMatrix());

		// Switch polygon mode
		if (glfwGetKey(window, 'Z')) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (glfwGetKey(window, 'X')) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	demo->deinit();
	delete demo;

	// Close window and terminate GLFW
	glfwTerminate();
}
