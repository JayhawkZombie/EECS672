#include <stdlib.h>

#include "CustomGLFWController.h"
#include "GLFWController.h"
#include "ModelView.h"
#include "ModelViewWithPhongLighting.h"

int CustomGLFWController::clickedX = 0;
int CustomGLFWController::clickedY = 0;
bool CustomGLFWController::didClickLeft = false;

CustomGLFWController::CustomGLFWController(const std::string &windowTitle, int rcFlags) : GLFWController(windowTitle, rcFlags)
{

}

void CustomGLFWController::handleMouseButton(Controller::MouseButton button, bool isPressed, int x, int y, int mods)
{
	if (button == LEFT_BUTTON && isPressed)
	{
		CustomGLFWController::clickedX = x;
		CustomGLFWController::clickedY = y;
		CustomGLFWController::didClickLeft = true;
	}
	else
	{
		CustomGLFWController::didClickLeft = false;
	}
}

void CustomGLFWController::handleMouseMotion(int x, int y)
{
	if (CustomGLFWController::didClickLeft)
	{
		float dX = x - clickedX;
		float dY = y - clickedY;

		ModelViewWithPhongLighting::addToGlobalRotationDegrees(dY / 5, dX / 5, 0);
		redraw();

		clickedX = x;
		clickedY = y;
	}
	else
	{

	}
}

void CustomGLFWController::handleScroll(bool isUp)
{

	std::cerr << "Handling scrolling" << std::endl;

	float value = 0.2;
	if (isUp)
	{
		value *= 1;
	}
	else
	{
		value *= -1;
	}
	ModelViewWithPhongLighting::addToGlobalZoom( value );
}

void CustomGLFWController::handleAsciiChar(unsigned char charPressed, int x, int y)
{
	if (charPressed == 'o')
	{
		ModelView::setProjection(ORTHOGONAL);
	}
	else if (charPressed == 'p')
	{
		ModelView::setProjection(PERSPECTIVE);
	}
	else if (charPressed == 'q')
	{
		ModelView::setProjection(OBLIQUE);
	}
}

void CustomGLFWController::handleFunctionKey(int fKey, int x, int y, int mods)
{
	Controller::handleFunctionKey(fKey, x, y, mods);
}

void CustomGLFWController::handleSpecialKey(Controller::SpecialKey key, int x, int y, int mods)
{
	Controller::handleSpecialKey(key, x, y, mods);
}
/*
void CustomGLFWController::handleDisplay()
{
	GLFWController::handleDisplay();
	
	glfwMakeContextCurrent(theWindow);
	int width;
	int height;
	glfwGetFramebufferSize(theWindow, &width, &height);
	glViewport(0, 0, width, height);

	glClear(glClearFlags);

	int w = 0;

	drawAllObjects(); 

} */

void CustomGLFWController::drawAllObjects()
{
	for (std::vector<ModelView*>::iterator it = models.begin(); it < models.end(); it++)
	{
		(*it)->render();
	}
}
