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
	glm::mat4x4 m_cameraMatrix;
	glm::vec4 m_camera;
	glm::mat4x4 m_spriteScale;
	unsigned int m_width;
	unsigned int m_height;

	GLuint m_basicShader;
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

	void SetCameraZoom(float zoom);
	float GetCameraZoom();

	bool DrawSprite(Sprite* sprite, const glm::mat4& matrix);
	bool DrawSprite(AnimatedSprite* sprite, const glm::mat4& matrix);
	
	void DrawTileset(unsigned int vbo, unsigned int ibo, unsigned int vertices, Sprite* sprite, const glm::mat4& matrix);

	void PreRender();
	void PostRender();

	bool IsRectVisible(float x, float y, float w, float h);
};