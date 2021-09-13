#ifndef RESOURCE_MANAGER_HEADER
#define RESOURCE_MANAGER_HEADER

#include "windows.h"

class ResourceManager
{
public:
	static void Initialize();
	static HFONT GetDefaultFont(const bool& isBold = false);

private:
	ResourceManager() {};

	static HFONT m_DefaultFont;
	static HFONT m_DefaultBoldFont;
};
#endif