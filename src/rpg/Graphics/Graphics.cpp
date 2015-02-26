/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Graphics.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Graphics.h"

#include "Sprite.h"
#include "AnimatedSprite.h"

#include <Logger.h>

#include <Input/Input.h>

#define CATCH_ERROR\
		{unsigned int error = glGetError();\
	if (error != 0) {\
		Error("[graphics] %s:%d %d:%s", __FILE__, __LINE__, error, gluErrorString(error));\
			}}

struct Vertex
{
	float x, y;
	unsigned int color;
	float u, v;
};

Graphics::Graphics()
	: m_window(NULL), m_camera(0, 0, 0, 1), m_width(800), m_height(600)
{
	glfwSetErrorCallback(Graphics::ErrorCallback);
	if (glfwInit())
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_window = glfwCreateWindow(800, 600, "RPG", NULL, NULL);
		if (m_window)
		{		
			// Initialize input
			Input::Init(m_window);

			glfwSetWindowUserPointer(m_window, this);
			glfwSetFramebufferSizeCallback(m_window, Graphics::ResizeCallback);
			glfwMakeContextCurrent(m_window);
			glfwSwapInterval(0);
			CATCH_ERROR
			// Setup extensions
			glewExperimental = GL_TRUE;
			glewInit();

			// Hack for OGL - VBO seems to be not working without it :-)
			unsigned int HackVAO;
			glGenVertexArrays(1, &HackVAO);
			glBindVertexArray(HackVAO);

			CATCH_ERROR
			glDisable(GL_DEPTH_TEST);
			CATCH_ERROR
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			CATCH_ERROR

			glClearColor(0, 0, 0, 1);
			Resize(800, 600);
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
				delete[]buffer;
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
				delete[]buffer;
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
				delete[]errorLog;
			}

			CATCH_ERROR
			glCreateBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, NULL, GL_DYNAMIC_DRAW); // GL_DYNAMIC_DRAW because we are updating vbo every sprite.
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

void Graphics::UpdateCameraMatrix()
{	
	m_cameraMatrix = glm::translate(glm::mat4(), glm::vec3(floor(-m_camera.x), floor(-m_camera.y), 0)) * glm::translate(glm::mat4(), glm::vec3(floor(m_width / 2 * m_camera.w), floor(m_height / 2 * m_camera.w), 0));
	m_cameraMatrix[3][3] = m_camera.w;
}

void Graphics::ErrorCallback(int error, const char* description)
{
	Error("[graphics] [%d] %s", error, description);
}

bool IsPowerOf2(int x)
{
	return (x != 0);
}

void Graphics::ResizeCallback(GLFWwindow* window, int width, int height)
{
	if (width < 800)
	{
		glfwSetWindowSize(window, 800, height);
		width = 800;
	}

	if (height < 600)
	{
		glfwSetWindowSize(window, width, 600);
		height = 600;
	}

	((Graphics *)glfwGetWindowUserPointer(window))->Resize(width, height);
}

void Graphics::Resize(int _width, int _height)
{
	m_width = _width;
	m_height = _height;

	UpdateCameraMatrix();
		
	glViewport(0, 0, m_width, m_height);
	m_orthoMatrix = glm::ortho(0.0f, (float)m_width, (float)m_height, 0.0f);

	m_spriteScale = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
}

bool Graphics::UpdateWindow()
{
	if (!m_window)
		return false;

	Input::Reset();
	glfwPollEvents();
	return !glfwWindowShouldClose(m_window);
}

void Graphics::SetWindowTitle(const char *title)
{
	glfwSetWindowTitle(m_window, title);
}


void Graphics::SetCameraPosition(const Vector2d& pos)
{
	m_camera.x = pos.x;
	m_camera.y = pos.y;
	UpdateCameraMatrix();
}

Vector2d Graphics::GetCameraPosition()
{
	return Vector2d(m_camera.x, m_camera.y);
}

void Graphics::SetCameraZoom(float zoom)
{
	m_camera.w = zoom;
	UpdateCameraMatrix();
}

float Graphics::GetCameraZoom()
{
	return m_camera.w;
}

bool Graphics::DrawSprite(Sprite* sprite, const glm::mat4& matrix)
{
	// Calculate mvp
	glm::mat4 model = m_cameraMatrix * glm::mat4(matrix);
	Vector2d size = sprite->GetSize();

	if (!IsRectVisible(model[3][0], model[3][1], size.x, size.y))
	{
		//Info("[cull] %f, %f", model[3][0], model[3][1]);
		return false;
	}
	glm::mat4 mvp = m_orthoMatrix * model;

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Update vbo to sprite dimensions
	Vertex *data = static_cast<Vertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	
	Vector2d halfSize = size / 2.0f;

	unsigned int color = 0xFFFFFFFF;
	float opacity = sprite->GetOpacity();
	if (opacity < 1.0f)
		color = 0x00FFFFFF | (static_cast<unsigned int>(floor(255.0f * opacity)) << 24);			

	Vertex v[4] = {
		{ -halfSize.x, -halfSize.y, color, 0.0f, 1.0f },
		{ -halfSize.x, halfSize.y, color, 0.0f, 0.0f },
		{ halfSize.x, -halfSize.y, color, 1.0f, 1.0f },
		{ halfSize.x, halfSize.y, color, 1.0f, 0.0f }
	};

	memcpy(data, v, sizeof(Vertex) * 4);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Set basic shader as current program
	glUseProgram(m_basicShader);

	// Set model view projection matrix
	unsigned int position = glGetUniformLocation(m_basicShader, "mvp");
	if (position != -1)
		glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(mvp));

	// Set texture
	unsigned int textureId = sprite->GetTextureId();
	if (textureId != -1 && glIsTexture(textureId))
	{
		unsigned int texPosition = glGetUniformLocation(m_basicShader, "texture");
		if (texPosition != -1)
		{			
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glUniform1i(texPosition, 0);			
		}
	}

	// Grab vertex position attrib location
	unsigned int aPosition = glGetAttribLocation(m_basicShader, "vertexPosition");
	if (aPosition != -1)
		glEnableVertexAttribArray(aPosition);

	// Grab vertex color attrib location
	unsigned int aColor = glGetAttribLocation(m_basicShader, "vertexColor");
	if (aColor != -1)
		glEnableVertexAttribArray(aColor);

	// Grab vertex uv attrib location
	unsigned int aUV = glGetAttribLocation(m_basicShader, "vertexUV");
	if (aUV != -1)
		glEnableVertexAttribArray(aUV);
	
	// Set pointer to position
	if (aPosition != -1)
		glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Set pointer to color
	if (aColor != -1)
		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

	// Set pointer to uv
	if (aUV != -1)
		glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));

	// Draw our vbo
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Unbind array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Disable attribs
	if (aPosition != -1)
		glDisableVertexAttribArray(aPosition);

	if (aColor != -1)
		glDisableVertexAttribArray(aColor);

	if (aUV != -1)
		glDisableVertexAttribArray(aUV);

	// Disable blending
	glDisable(GL_BLEND);
	return true;
}

bool Graphics::DrawSprite(AnimatedSprite* sprite, const glm::mat4& matrix)
{
	// Calculate mvp
	glm::mat4 model = m_cameraMatrix * glm::mat4(matrix);
	Vector2d size = sprite->GetSize();

	if (!IsRectVisible(model[3][0], model[3][1], size.x, size.y))
	{
		return false;
	}
	glm::mat4 mvp = m_orthoMatrix * model;

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Update vbo to sprite dimensions
	Vertex *data = static_cast<Vertex *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

	Vector2d halfSize = size / 2.0f;

	AnimationFrame frame = sprite->GetCurrentFrameData();

	unsigned int color = 0xFFFFFFFF;
	float opacity = sprite->GetOpacity();
	if (opacity < 1.0f)
		color = 0x00FFFFFF | (static_cast<unsigned int>(floor(255.0f * opacity)) << 24);

	Vertex v[4] = {
		{ -halfSize.x, -halfSize.y, color, frame.x, frame.y },
		{ -halfSize.x, halfSize.y, color, frame.x, frame.h },
		{ halfSize.x, -halfSize.y, color, frame.w, frame.y },
		{ halfSize.x, halfSize.y, color, frame.w, frame.h }
	};

	memcpy(data, v, sizeof(Vertex) * 4);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Set basic shader as current program
	glUseProgram(m_basicShader);

	// Set model view projection matrix
	unsigned int position = glGetUniformLocation(m_basicShader, "mvp");
	if (position != -1)
		glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(mvp));

	// Set texture
	unsigned int textureId = sprite->GetTextureId();
	if (textureId != -1 && glIsTexture(textureId))
	{
		unsigned int texPosition = glGetUniformLocation(m_basicShader, "texture");
		if (texPosition != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glUniform1i(texPosition, 0);
		}
	}

	// Grab vertex position attrib location
	unsigned int aPosition = glGetAttribLocation(m_basicShader, "vertexPosition");
	if (aPosition != -1)
		glEnableVertexAttribArray(aPosition);

	// Grab vertex color attrib location
	unsigned int aColor = glGetAttribLocation(m_basicShader, "vertexColor");
	if (aColor != -1)
		glEnableVertexAttribArray(aColor);

	// Grab vertex uv attrib location
	unsigned int aUV = glGetAttribLocation(m_basicShader, "vertexUV");
	if (aUV != -1)
		glEnableVertexAttribArray(aUV);

	// Set pointer to position
	if (aPosition != -1)
		glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Set pointer to color
	if (aColor != -1)
		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

	// Set pointer to uv
	if (aUV != -1)
		glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));

	// Draw our vbo
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Unbind array buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Disable attribs
	if (aPosition != -1)
		glDisableVertexAttribArray(aPosition);

	if (aColor != -1)
		glDisableVertexAttribArray(aColor);

	if (aUV != -1)
		glDisableVertexAttribArray(aUV);

	// Disable blending
	glDisable(GL_BLEND);
	return true;
}

void Graphics::DrawTileset(unsigned int vbo, unsigned int ibo, unsigned int vertices, Sprite* sprite, const glm::mat4& matrix)
{
	// Calculate mvp
	glm::mat4 mvp = m_orthoMatrix * m_cameraMatrix * glm::mat4(matrix);

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Set basic shader as current program
	glUseProgram(m_basicShader);

	// Set model view projection matrix
	unsigned int position = glGetUniformLocation(m_basicShader, "mvp");
	if (position != -1)
		glUniformMatrix4fv(position, 1, GL_FALSE, glm::value_ptr(mvp));

	// Set texture
	unsigned int textureId = sprite->GetTextureId();
	if (textureId != -1 && glIsTexture(textureId))
	{
		unsigned int texPosition = glGetUniformLocation(m_basicShader, "texture");
		if (texPosition != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glUniform1i(texPosition, 0);
		}
	}

	// Grab vertex position attrib location
	unsigned int aPosition = glGetAttribLocation(m_basicShader, "vertexPosition");
	if (aPosition != -1)
		glEnableVertexAttribArray(aPosition);

	// Grab vertex color attrib location
	unsigned int aColor = glGetAttribLocation(m_basicShader, "vertexColor");
	if (aColor != -1)
		glEnableVertexAttribArray(aColor);

	// Grab vertex uv attrib location
	unsigned int aUV = glGetAttribLocation(m_basicShader, "vertexUV");
	if (aUV != -1)
		glEnableVertexAttribArray(aUV);

	// Set pointer to position
	if (aPosition != -1)
		glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Set pointer to color
	if (aColor != -1)
		glVertexAttribPointer(aColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

	// Set pointer to uv
	if (aUV != -1)
		glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Draw our vbo	
	glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);

	// Disable attribs
	if (aPosition != -1)
		glDisableVertexAttribArray(aPosition);

	if (aColor != -1)
		glDisableVertexAttribArray(aColor);

	if (aUV != -1)
		glDisableVertexAttribArray(aUV);

	// Disable blending
	glDisable(GL_BLEND);
}

void Graphics::PreRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::PostRender()
{
	glfwSwapBuffers(m_window);
}

bool Graphics::IsRectVisible(float x, float y, float w, float h)
{
	return ((x >= 0 && x <= m_width * m_camera.w) || ((x + w) >= 0 && (x + w) <= m_width * m_camera.w))
		&& ((y >= 0 && y <= m_height * m_camera.w) || ((y + h) >= 0 && (y + h) <= m_height * m_camera.w));
}