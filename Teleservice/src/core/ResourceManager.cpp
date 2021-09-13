#include "ResourceManager.h"

void ResourceManager::Initialize()
{
	// Create the default font to be used in the application
	m_DefaultFont = CreateFont(-11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Microsoft Sans Serif");
	m_DefaultBoldFont = CreateFont(-11, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, L"Microsoft Sans Serif");
}

HFONT ResourceManager::GetDefaultFont(const bool& isBold /*= false*/)
{
	return isBold ? m_DefaultBoldFont : m_DefaultFont;
}

HFONT ResourceManager::m_DefaultFont;
HFONT ResourceManager::m_DefaultBoldFont;
