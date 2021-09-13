#include "Renderer.h"

void Renderer::LoadOpenGL()
{
	gladLoadGL();
	m_IsOpenGLLoaded = true;
}

bool Renderer::m_IsOpenGLLoaded;
