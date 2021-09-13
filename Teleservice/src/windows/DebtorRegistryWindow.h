#ifndef DEBTOR_REGISTRY_WINDOW_HEADER
#define DEBTOR_REGISTRY_WINDOW_HEADER

#include "core/Window.h"

class DebtorRegistryWindow : public Window
{
public:
	DebtorRegistryWindow(const HINSTANCE& instance) : Window(L"Kundregister", L"DebtorRegistryWindow", 512, 360) {};

	void OnCreate() override;
};
#endif