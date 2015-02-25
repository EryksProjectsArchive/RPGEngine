/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Graphics.cpp
* Author: Eryk Dwornicki
*
*****************************************************/
#include "Graphics.h"

#include <Logger.h>

#define CATCH_ERROR\
		{unsigned int error = glGetError();\
	if (error != 0) {\
		Error("[graphics] %s:%d %d:%s", __FILE__, __LINE__, error, gluErrorString(error));\
			}}

struct Vertex
{
	float x, y;
	unsigned int color;
};

Graphics::Graphics()
	: m_window(NULL), m_camera(0, 0, 0, 1)
{
	glfwSetErrorCallback(Graphics::ErrorCallback);

	if (glfwInit())
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_window = glfwCreateWindow(800, 600, "RPG", NULL, NULL);
		if (m_window)
		{
			glfwMakeContextCurrent(m_window);
			CATCH_ERROR
			// Setup extensions
			glewExperimental = GL_TRUE;
			glewInit();

			unsigned int HackVAO;
			glGenVertexArrays(1, &HackVAO);
			glBindVertexArray(HackVAO);

			CATCH_ERROR
			glDisable(GL_DEPTH_TEST);
			CATCH_ERROR
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			CATCH_ERROR
			glClearColor(0, 0, 0, 1);
			glViewport(0, 0, 800, 600);

			m_orthoMatrix = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
			SetCameraPosition(Vector2d(0, 0));

			m_basicShader = glCreateProgram();
			CATCH_ERROR
			FILE *fp = fopen("../Data/Shaders/Simple.vert", "rb");
			if (fp)
			{
				GLuint vs = glCreateShader(GL_VERTEX_SHADER);
				fseek(fp, 0, SEEK_END);
				long size = ftell(fp);
				rewind(fp);

				char* buffer = new char[size+1];
				memset(buffer, 0, sizeof(buffer));
				fread(buffer, sizeof(char), size, fp);
				buffer[size] = '\0';

				fclose(fp);

				glShaderSource(vs, 1, &buffer, NULL);
				glCompileShader(vs);
				glAttachShader(m_basicShader, vs);
			}

			CATCH_ERROR
			fp = fopen("../Data/Shaders/Simple.frag", "rb");
			if (fp)
			{
				GLuint vs = glCreateShader(GL_FRAGMENT_SHADER);
				fseek(fp, 0, SEEK_END);
				long size = ftell(fp);
				rewind(fp);

				char* buffer = new char[size+1];
				memset(buffer, 0, sizeof(buffer));
				fread(buffer, sizeof(char), size, fp);
				buffer[size] = '\0';

				fclose(fp);

				glShaderSource(vs, 1, &buffer, NULL);
				glCompileShader(vs);
				glAttachShader(m_basicShader, vs);
			}

			glLinkProgram(m_basicShader);
			CATCH_ERROR
			GLint params = 0;
			glGetProgramiv(m_basicShader, GL_LINK_STATUS, &params);
			if (params == GL_FALSE)
			{
				Error("[graphics] Unable to compile shader.");

				glGetProgramiv(m_basicShader, GL_INFO_LOG_LENGTH, &params);
				char *errorLog = new char[params + 1];
				glGetProgramInfoLog(m_basicShader, params, &params, errorLog);
				errorLog[params] = '\0';

				Error("[graphics] %s", errorLog);
			}

			CATCH_ERROR
			glCreateBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			Vertex v[4] = {
				{ -50.0f, -50.0f, 0xFF0000FF },
				{ -50.0f, 50.0f, 0xFF0000FF },
				{ 50.0f, -50.0f, 0xFF0000FF },
				{ 50.0f, 50.0f, 0xFF0000FF }
				
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, v, GL_STATIC_DRAW);


			CATCH_ERROR
		} else {

			glfwTerminate();
			Error("[graphics] Unable to create window");
		}
	}
	else
	{
		Error("[graphics] Cannot initialize glfw");
	}
}

Graphics::~Graphics()
{
	glfwTerminate();
}

void Graphics::ErrorCallback(int error, const char* description)
{
	Error("[graphics] [%d] %s", error, description);
}

bool Graphics::UpdateWindow()
{
	if (!m_window)
		return false;

	glfwPollEvents();
	return !glfwWindowShouldClose(m_window);
}

void Graphics::SetCameraPosition(const Vector2d& pos)
{
	m_camera.x = pos.x;
	m_camera.y = pos.y;
}

Vector2d Graphics::GetCameraPosition()
{
	return Vector2d(m_camera.x, m_camera.y);
}

void Graphics::SetCameraZoom(float zoom)
{
	m_camera.w = zoom;
}

float Graphics::GetCameraZoom()
{
	return m_camera.w;
}

void Graphics::DrawSprite(Sprite* sprite, const glm::mat4& matrix)
{
	// Calculate mvp
	glm::mat4 cam;
	cam[3] = m_camera;

	glm::mat4 mvp = m_orthoMatrix * cam * glm::mat4(matrix);

	// Set basic shader as current program
	glUseProgram(m_basicShader);	

	// Set model view projection matrix
	unsigned int position = glGetUniformLocation(m_basicShader, "mvp");
	if (position != -1)
		glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(mvp));

	// Grab vertex position attrib location
	unsigned int aPosition = glGetAttribLocation(m_basicShader, "vertexPosition");
	if (aPosition != -1)
		glEnableVertexAttribArray(aPosition);

	// Grab vertex color attrib location
	unsigned int aColor = glGetAttribLocation(m_basicShader, "vertexColor");
	if (aColor != -1)
		glEnableVertexAttribArray(aColor);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Set pointer to position
	if (aPosition != -1)
		glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Set pointer to color
	if (aColor != -1)
		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

	// Draw our vbo
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Unbind array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Disable attribs
	if (aPosition != -1)
		glDisableVertexAttribArray(aPosition);

	if (aColor != -1)
		glDisableVertexAttribArray(aColor);
}

void Graphics::PreRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::PostRender()
{
	glfwSwapBuffers(m_window);
}