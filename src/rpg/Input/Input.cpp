/*****************************************************
*
* RPG - 2d game project
* File: Input/Input.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Input.h"

GLFWwindow* Input::ms_window = NULL;
float Input::ms_scroll = 0.0f;

void Input::Init(GLFWwindow* window)
{
	ms_window = window;
	glfwSetScrollCallback(window, Input::ScrollCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/* Internal callbacks */
void Input::ScrollCallback(GLFWwindow* window, double x, double y)
{
	ms_scroll = static_cast<float>(y);
}

/* Get functions that can be used from any place of code */
float Input::GetMouseScroll()
{
	return ms_scroll;
}

Vector2d Input::GetMousePosition(bool absolute)
{
	double x, y;
	glfwGetCursorPos(ms_window, &x, &y);

	/* If we set absolute argument to true we do not clamp the mouse position to window bounds */
	if (!absolute)
	{
		int w, h;
		glfwGetWindowSize(ms_window, &w, &h);
		x = (x < 0.0) ? 0.0 : ((x > (double)w) ? w : x);
		y = (y < 0.0) ? 0.0 : ((y > (double)h) ? h : y);
	}

	return Vector2d(static_cast<float>(x), static_cast<float>(y));
}

bool Input::IsKeyDown(int key)
{
	return glfwGetKey(ms_window, key) == GLFW_PRESS;
}

void Input::Reset()
{
	ms_scroll = 0.0f;
}