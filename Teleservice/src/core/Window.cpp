#include "resource.h"
#include "Window.h"
#include <iostream>
#include "Application.h"
#include "ui/Button.h"
#include "ui/ComboBox.h"
#include "ui/CheckBox.h"
#include "ui/TextBox.h"
#include "ui/Label.h"
#include "ui/Menu.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "pdfium/fpdfview.h"

Window::Window(const std::wstring& windowTitle, const std::wstring& windowClassName, const int& width, const int& height)
	: m_Title(windowTitle), m_ClassName(windowClassName), m_Width(width), m_Height(height)
{
	WindowManager::AddWindow(this);
}

Window::~Window()
{
	m_Controls.erase(m_Controls.begin(), m_Controls.end());

	// Delete window handle
	DeleteObject(m_WindowHandle);
}

void Window::ConstructWindow(const HINSTANCE& hInstance)
{
	m_WindowInstance = hInstance;

	OnCreate();
	RegisterWindow();
}

Control* Window::GetControlByID(const int& id) const
{
	auto result = m_Controls.find(m_ControlNames[id]);

	if (result == m_Controls.end())
	{
		return nullptr;
	}

	return result->second;
}

void Window::DestroyAllControls()
{
	EnumChildWindows(Application::MainWindow->GetHandle(), DestroyChildCallback, NULL);
}

void Window::OnCreate()
{
	
}

void Window::OnStart()
{

}

void Window::OnUpdate()
{

}

void Window::OnRender()
{

}

LRESULT Window::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

Control* Window::AddControl(const std::string& controlName, Control* control)
{
	if (m_Controls.find(controlName) == m_Controls.end())
	{
		control->SetID(GetControlCount());
		m_Controls.insert({ controlName, control });
		m_ControlNames.push_back(controlName);
		
		return control;
	}

	return nullptr;
}

void Window::RegisterWindow()
{
	// Register window class
	m_WindowClass = {};
	m_WindowClass.cbSize = sizeof(WNDCLASSEXW);
	m_WindowClass.style = NULL;
	m_WindowClass.lpfnWndProc = WndProcedure;
	m_WindowClass.cbClsExtra = NULL;
	m_WindowClass.cbWndExtra = NULL;
	m_WindowClass.hInstance = m_WindowInstance;
	m_WindowClass.hbrBackground = m_HBRBackground;
	m_WindowClass.lpszClassName = m_ClassName.c_str(); //needs to be the same name when creating the window as well
	m_WindowClass.hIcon = LoadIconW(m_WindowInstance, MAKEINTRESOURCE(IDI_ICON1));
	m_WindowClass.hIconSm = NULL;
	m_WindowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);

	// register the class
	if (!RegisterClassExW(&m_WindowClass))
	{
		MessageBox(NULL, L"Could not register window class!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
	}

	// Create window handle
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = 1280;
	rect.bottom = 720;
	AdjustWindowRect(&rect, m_StyleAttributes, false);

	m_WindowHandle = CreateWindowExW(
				   m_ExtendedStyleAttributes, // dwExStyle
				   m_ClassName.c_str(),  // lpClassName
				   m_Title.c_str(), // lpWindowName
				   m_StyleAttributes,   // dwStyle
				   CW_USEDEFAULT,         // X
				   CW_USEDEFAULT,         // Y
				   rect.right - rect.left,                  // nWidth
				   rect.bottom - rect.top,                   // nHeight
				   NULL,                  // hWndParent
				   NULL,                  // hmenu
				   m_WindowInstance,            // hInstance
				   this                   // lpParam
				   );

	if (m_MenuBar != nullptr)
	{
		m_MenuBar->Create(m_WindowHandle);
	}

	// Create all controls
	for (auto it = m_Controls.begin(); it != m_Controls.end(); it++)
	{
		it->second->Create(m_WindowHandle);
	}

	if (m_MenuBar != nullptr)
	{
		//SetMenu(m_WindowHandle, m_MenuBar->GetMenuHandle());
	}
}

void Window::PreConstructWindow()
{
	// Construct OpenGL renderer
	if (m_RenderingAPI == RenderingAPI::OpenGL)
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
			1,                     // version number  
			PFD_DRAW_TO_WINDOW |   // support window  
			PFD_SUPPORT_OPENGL |   // support OpenGL  
			PFD_DOUBLEBUFFER,      // double buffered  
			PFD_TYPE_RGBA,         // RGBA type  
			32,                    // 32-bit color depth  
			0, 0, 0, 0, 0, 0,      // color bits ignored  
			0,                     // no alpha buffer  
			0,                     // shift bit ignored  
			0,                     // no accumulation buffer  
			0, 0, 0, 0,            // accum bits ignored  
			0,                     // 32-bit z-buffer      
			0,                     // no stencil buffer  
			0,                     // no auxiliary buffer  
			PFD_MAIN_PLANE,        // main layer  
			0,                     // reserved  
			0, 0, 0                // layer masks ignored
		};

		// Device-context creation
		HDC deviceContext = GetDC(m_WindowHandle);
		SetDeviceContext(deviceContext);

		// Pixel format
		int pixelFormat = ChoosePixelFormat(GetDeviceContext(), &pfd);
		SetPixelFormat(deviceContext, pixelFormat, &pfd);

		// Rendering-context creation
		HGLRC renderingContext = wglCreateContext(deviceContext);
		SetRenderingContext(renderingContext);
		wglMakeCurrent(deviceContext, renderingContext);

		// Load GLAD (if not already loaded)
		if (!Renderer::IsRenderingAPILoaded(m_RenderingAPI))
		{
			Renderer::LoadOpenGL();
		}

		glViewport(0, 0, GetWidth(), GetHeight());
		((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1);
	}
}

LRESULT CALLBACK Window::WndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* currentWindow = nullptr;

	// Trick for setting window handle early (before CreateWindowEx has returned it)
	if (message == WM_NCCREATE) // the very first message to be sent
	{
		LPCREATESTRUCTW lpcs = reinterpret_cast<LPCREATESTRUCTW>(lParam);
		currentWindow = static_cast<Window*>(lpcs->lpCreateParams);
		currentWindow->SetHandle(hWnd);
		currentWindow->PreConstructWindow();
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(currentWindow));
	}
	else
	{
		// since WM_NCCREATE is the first message to be sent,
		// we can guarantee that the window long pointer has
		// been set in the if-statement above. In that case
		// set the currentWindow variable by getting the
		// window long pointer stored in the extra bytes instead
		currentWindow = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	}

	// Then send the message to the current window's WndProc
	if (currentWindow != nullptr)
	{
		return currentWindow->WindowProcedure(hWnd, message, wParam, lParam);
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

bool CALLBACK Window::SetFont(HWND child, LPARAM font)
{
	SendMessage(child, WM_SETFONT, font, true);
	return true;
}

BOOL CALLBACK Window::DestroyChildCallback(HWND hwnd, LPARAM lParam)
{
	if (hwnd != NULL)
	{
		DestroyWindow(hwnd);
	}

	return TRUE;
}