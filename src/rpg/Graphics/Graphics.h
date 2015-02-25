/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Graphics.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <Math/Math.h>
#include <Math/Vector2d.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Graphics
{
private:
	GLFWwindow * m_window;
	glm::mat4x4 m_orthoMatrix;
	glm::vec4 m_camera;

	GLuint m_basicShader;
	GLuint m_vbo;

	static void ErrorCallback(int error, const char* description);
public:
	Graphics();
	~Graphics();

	bool UpdateWindow();

	void SetCameraPosition(const Vector2d& pos);
	Vector2d GetCameraPosition();

	void SetCameraZoom(float zoom);
	float GetCameraZoom();

	void DrawSprite(Sprite* sprite, const glm::mat4& matrix);

	void PreRender();
	void PostRender();
};