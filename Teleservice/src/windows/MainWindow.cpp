#include "MainWindow.h"
#include "core/WindowManager.h"
#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/TextBox.h"
#include <dwmapi.h>
#include <cassert>

void MainWindow::OnCreate()
{
	// Styles
	AddStyle(WindowStyle::InitiallyVisible);
	m_HBRBackground = (HBRUSH)(COLOR_BTNFACE + 1);

	// Menu bar
	m_MenuBar = new MenuItem(L"");

	// File
	MenuItem* fileMenu = (MenuItem*)AddControl("FileMenu", new MenuItem(L"&Arkiv", m_MenuBar));
	MenuItem* fileMenuOpen = (MenuItem*)AddControl("FileMenuOpenItem", new MenuItem(L"&Öppna\tCtrl+O", fileMenu)); // File -> Open
	MenuItem* fileMenuSave = (MenuItem*)AddControl("FileMenuSaveItem", new MenuItem(L"&Spara\tCtrl+S", fileMenu)); // File -> Save

	// View
	MenuItem* viewMenu = (MenuItem*)AddControl("ViewMenu", new MenuItem(L"&Visa", m_MenuBar));
	MenuItem* viewMenuCustomers = (MenuItem*)AddControl("ViewMenuCustomers", new MenuItem(L"&Kundregister", viewMenu));

	// Ärendenummer
	//AddControl("CustomerIDLabel", new Label(L"Ärendenummer", 10, 5));
	//AddControl("CustomerIDTextBox", new TextBox(90, 2, 100, 20));

	// -------- Gäldenär --------
	//AddControl("ApplicantHeaderLabel", new Label(L"Gäldenär", 10, 40, true));

	// Personnummer
	AddControl("CustomerSocialIDLabel", new Label(L"Personnummer", 10, 60));
	AddControl("CustomerSocialIDTextBox", new TextBox(100, 57, 200, 20));

	// Namn
	AddControl("CustomerNameLabel", new Label(L"Namn", 10, 90));
	AddControl("CustomerNameTextBox", new TextBox(100, 87, 200, 20));

	// Adress
	AddControl("CustomerAddressLabel", new Label(L"Adress", 10, 120));
	AddControl("CustomerAddressTextBox", new TextBox(100, 117, 200, 20));

	// Postadress
	AddControl("CustomerPostalLabel", new Label(L"Postnummer", 10, 150));
	AddControl("CustomerPostalTextBox", new TextBox(100, 147, 200, 20));

	// Ort
	AddControl("CustomerCityLabel", new Label(L"Ort", 10, 180));
	AddControl("CustomerCityTextBox", new TextBox(100, 177, 200, 20));

	// Telefonnummer
	AddControl("CustomerPhoneNumberLabel", new Label(L"Telefonnummer", 10, 210));
	AddControl("CustomerPhoneNumberTextBox", new TextBox(100, 207, 200, 20));

	AddControl("CustomerAddButton", new Button(L"Lägg till kund", 50, 250, 80, 20, DestroyAllControls));
	AddControl("CustomerCancelButton", new Button(L"Avbryt", 150, 250, 80, 20));
}

void MainWindow::OnSize()
{

}

void MainWindow::OnActivate()
{
	// Extend the frame into the client area.
	MARGINS margins;

	margins.cxLeftWidth = 8;
	margins.cxRightWidth = 8;
	margins.cyBottomHeight = 8;
	margins.cyTopHeight = 40;

	DwmExtendFrameIntoClientArea(m_WindowHandle, &margins);
}

LRESULT MainWindow::WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps{};
	HDC hdc{};
	LRESULT returnValue = 0;
	bool callDefaultWinProc = true;
	int leftExtend = 8;
	int rightExtend = 8;
	int bottomExtend = 10;
	int topExtend = 27;

	switch (message)
	{
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		break;
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

MainWindow* MainWindow::m_Instance = new MainWindow();