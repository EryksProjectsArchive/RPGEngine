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
#include <Math/Rectangle.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Color.h"

class Graphics
{
private:
	GLFWwindow * m_window;
	glm::mat4x4 m_orthoMatrix;
	glm::mat4x4 m_cameraMatrix;
	glm::vec4 m_camera;
	glm::mat4x4 m_spriteScale;
	unsigned int m_width;
	unsigned int m_height;
	Rectangle m_screenRectangle;

	GLuint m_basicShader;
	GLuint m_basicColorShader;
	GLuint m_vbo;

	static void ErrorCallback(int error, const char* description);
	static void ResizeCallback(GLFWwindow* window, int width, int height);

	void UpdateCameraMatrix();
	void Resize(int width, int height);
public:
	Graphics();
	~Graphics();

	bool UpdateWindow();
	void SetWindowTitle(const char *title);

	void SetCameraPosition(const Vector2d& pos);
	Vector2d GetCameraPosition();

	GLuint CreateProgram(const char * vertexShader, const char * fragmentShader);

	void SetCameraZoom(float zoom);
	float GetCameraZoom();

	bool DrawSprite(Sprite* sprite, const glm::mat4& matrix);
	bool DrawSprite(AnimatedSprite* sprite, const glm::mat4& matrix);

	void DrawSpritePart(Sprite *sprite, const Rectangle& rect, const Vector2d& uv1, const Vector2d& uv2, const Color& color);
		
	bool DrawRectangle(const Rectangle& rect, const Color& color);

	void DrawTileset(unsigned int vbo, unsigned int ibo, unsigned int vertices, Sprite* sprite, const glm::mat4& matrix);

	void PreRender();
	void PostRender();

	bool IsVisible(const Rectangle& rect);
};