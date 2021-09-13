#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "ui/Control.h"
#include "ui/MenuItem.h"
#include "rendering/Renderer.h"
#include "rendering/WindowStyle.h"
#include "rendering/ExtendedWindowStyle.h"

class Window
{
public:
	Window(const std::wstring& windowTitle, const std::wstring& windowClassName, const int& width, const int& height);
	virtual ~Window();

	void ConstructWindow(const HINSTANCE& hInstance);
	void PreConstructWindow();

	inline void AddStyle(const WindowStyle& style) { m_StyleAttributes |= (DWORD)style; }
	inline void AddExtendedStyle(const ExtendedWindowStyle& style) { m_ExtendedStyleAttributes |= (DWORD)style; }

	inline void Show() const { ShowWindow(m_WindowHandle, SW_RESTORE); }
	inline void Close() const { CloseWindow(m_WindowHandle); }
	inline HWND GetHandle() const { return m_WindowHandle; }
	inline void SetHandle(const HWND& handle) { m_WindowHandle = handle; }
	inline std::wstring GetTitle() const { return m_Title; }
	inline int GetControlCount() const { return m_Controls.size(); }

	inline HDC GetDeviceContext() const { return m_DeviceContext; }
	inline void SetDeviceContext(const HDC& deviceContext) { m_DeviceContext = deviceContext; }
	inline HGLRC GetRenderingContext() const { return m_RenderingContext; }
	inline void SetRenderingContext(const HGLRC& renderingContext) { m_RenderingContext = renderingContext; }

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	Control* GetControlByID(const int& id) const;
	static void DestroyAllControls();

	virtual void OnCreate();
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnSize() {};
	virtual void OnMove() {};
	virtual void OnNCCalcSize() {};
	virtual void OnActivate() {};

	virtual LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Control* AddControl(const std::string& controlName, Control* control);

protected:
	int m_Width;
	int m_Height;
	HINSTANCE m_WindowInstance;
	HWND m_WindowHandle;
	WNDCLASSEXW m_WindowClass;
	std::wstring m_Title;
	std::wstring m_ClassName;
	std::unordered_map<std::string, Control*> m_Controls;
	std::vector<std::string> m_ControlNames;
	MenuItem* m_MenuBar;

	// Properties
	RenderingAPI m_RenderingAPI = RenderingAPI::Win32;
	DWORD m_StyleAttributes;
	DWORD m_ExtendedStyleAttributes;

	// Rendering
	HDC m_DeviceContext;
	HGLRC m_RenderingContext;
	HBRUSH m_HBRBackground;
	Renderer m_Renderer;

private:
	void RegisterWindow();

	static BOOL CALLBACK DestroyChildCallback(HWND hwnd, LPARAM lParam);
	static LRESULT CALLBACK WndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool CALLBACK SetFont(HWND child, LPARAM font);
};
#endif