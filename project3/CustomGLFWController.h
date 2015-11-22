#ifndef CUSTOMCONTROLLER_H
#define CUSTOMCONTROLLER_H


#include "GLFW/glfw3.h"
#include "GLFWController.h"

class CustomGLFWController : public GLFWController
{
public:
	CustomGLFWController(const std::string &windowTitle, int rcFlags = 0);

	void handleMouseButton(MouseButton button, bool isPressed, int x, int y, int mods);
	void handleMouseMotion(int x, int y);
	void handleScroll(bool up);

	void handleAsciiChar(unsigned char charPressed, int x, int y);
	void handleFunctionKey(int fKey, int x, int y, int mods);
	void handleSpecialKey(SpecialKey key, int x, int y, int mods);
	//void handleDisplay();

private:
	static int clickedX;
	static int clickedY;
	static bool didClickLeft;

	void drawAllObjects();

};


#endif
