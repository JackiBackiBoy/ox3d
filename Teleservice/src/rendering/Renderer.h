#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "RenderingAPI.h"
#include <vendor/glad/glad.h>

class Renderer
{
public:
	Renderer() {};

	static void LoadOpenGL();
	inline static bool IsRenderingAPILoaded(const RenderingAPI& renderingAPI) { return m_IsOpenGLLoaded; }

private:
	static bool m_IsOpenGLLoaded;
};
#endif