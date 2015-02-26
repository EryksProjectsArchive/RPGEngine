/*****************************************************
*
* RPG - 2d game project
* File: Input/Input.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <GLFW/glfw3.h>
#include <Math/Vector2d.h>

class Input
{
private:
	static float ms_scroll;
	static GLFWwindow *ms_window;

	static void ScrollCallback(GLFWwindow* window, double x, double y);
public:
	static void Init(GLFWwindow* window);

	/**
	* @return Vertical scroll mouse value
	*/
	static float GetMouseScroll();

	/**
	 * @param absolute If set to <code>true</code> this function returns absolute screen space coordinates relative to top-left window corner. 
	 * @return Position of the mouse
	 */
	static Vector2d GetMousePosition(bool absolute = false);

	/**
	* @param key Key id - use GLFW_KEY_* macros.
	* @return Position of the mouse
	*/
	static bool IsKeyDown(int key);
	
	/**
	* This function resets input values - used only before pooling events from game window. (glfw is not calling scroll callback while scroll is equal 0)
	*/
	static void Reset();
};