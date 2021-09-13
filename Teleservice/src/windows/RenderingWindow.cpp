#include "RenderingWindow.h"
#include "math/Matrix4x4.h"
#include "math/Math.h"
#include "data/Mesh.h"

void RenderingWindow::OnCreate()
{
	AddStyle(WindowStyle::InitiallyVisible);
	AddStyle(WindowStyle::Caption);
	AddStyle(WindowStyle::Popup);
	AddStyle(WindowStyle::SizeBox);
	AddStyle(WindowStyle::MinimizeBox);
	AddStyle(WindowStyle::MaximizeBox);
	AddExtendedStyle(ExtendedWindowStyle::ClientEdge);

	m_RenderingAPI = RenderingAPI::OpenGL;
	m_HBRBackground = NULL; 
}

void RenderingWindow::OnStart()
{
	// Entities
	Mesh mesh;
	mesh.LoadFromFile("resources/models/monkey.obj");
	vertices = mesh.GetVertices();

	glEnable(GL_DEPTH_TEST);
	// VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	lightingShader.AttachShader(ShaderType::Vertex, "resources/shaders/lighting_shader.vert");
	lightingShader.AttachShader(ShaderType::Fragment, "resources/shaders/lighting_shader.frag");
	lightingShader.Create();

	
}

void RenderingWindow::OnUpdate()
{
	Matrix4x4 matrix = Matrix4x4::IdentityMatrix;
	matrix = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	float NearZ = 1.0f;
	float FarZ = 10.0f;

	float invTanHalfFov = 1 / (tanf(Math::ToRadians(90.0f / 2)));
	float A = (-FarZ - NearZ) / (NearZ - FarZ);
	float B = 2.0f * FarZ * NearZ / (NearZ - FarZ);

	RECT clientRect;
	GetClientRect(m_WindowHandle, &clientRect);

	float aspectRatio = (float)(clientRect.right - clientRect.left) / (clientRect.bottom - clientRect.top);

	Matrix4x4 projectionMatrix = { invTanHalfFov / aspectRatio, 0.0f,          0.0f, 0.0f,
								   0.0f,		                invTanHalfFov, 0.0f, 0.0f,
								   0.0f,		                0.0f,		   A,    B,
								   0.0f,		                0.0f,		   1.0f, 0.0f };

	lightingShader.SetUniformMat4("ProjectionMatrix", projectionMatrix);


	Matrix4x4 rotationMatrix = { cosf(Math::ToRadians(deg)),  0.0f, sinf(Math::ToRadians(deg)), 0.0f,
								 0.0f,                       1.0f, 0.0f,                      0.0f,
								 -sinf(Math::ToRadians(deg)), 0.0f, cosf(Math::ToRadians(deg)), 3.0f,
								 0.0f,                       0.0f, 0.0f,                      1.0f };

	lightingShader.SetUniformMat4("RotationMatrix", rotationMatrix);

	deg += 0.9f;
}

void RenderingWindow::OnRender()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingShader.Use();
	glBindVertexArray(m_VAO);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	SwapBuffers(m_DeviceContext);
}

LRESULT RenderingWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
		{
			Matrix4x4 matrix = Matrix4x4::IdentityMatrix;
			matrix = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			float NearZ = 1.0f;
			float FarZ = 10.0f;

			float invTanHalfFov = 1 / (tanf(Math::ToRadians(90.0f / 2)));
			float A = (-FarZ - NearZ) / (NearZ - FarZ);
			float B = 2.0f * FarZ * NearZ / (NearZ - FarZ);

			RECT clientRect;
			GetClientRect(m_WindowHandle, &clientRect);

			float aspectRatio = (float)(clientRect.right - clientRect.left) / (clientRect.bottom - clientRect.top);

			Matrix4x4 projectionMatrix = { invTanHalfFov / aspectRatio, 0.0f,          0.0f, 0.0f,
										   0.0f,		                invTanHalfFov, 0.0f, 0.0f,
										   0.0f,		                0.0f,		   A,    B,
										   0.0f,		                0.0f,		   1.0f, 0.0f };

			lightingShader.SetUniformMat4("ProjectionMatrix", projectionMatrix);

			glViewport(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
			OnRender(); // redraw the scene
			break;
		}
		case WM_PAINT:
		{
			return 0;
		}
		case WM_DESTROY:
		{
			ReleaseDC(hWnd, m_DeviceContext);
			wglDeleteContext(m_RenderingContext); // TODO: fix deletion of context
			PostQuitMessage(0);

			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

RenderingWindow* RenderingWindow::m_Instance = new RenderingWindow();