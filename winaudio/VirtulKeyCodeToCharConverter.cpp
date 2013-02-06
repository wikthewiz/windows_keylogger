#include "StdAfx.h"
#include "VirtulKeyCodeToCharConverter.h"

VirtulKeyCodeToCharConverter::VirtulKeyCodeToCharConverter(void)
{
}

VirtulKeyCodeToCharConverter::~VirtulKeyCodeToCharConverter(void)
{
}

int VirtulKeyCodeToCharConverter::getKeyboardLayoutFile(TCHAR* layoutFile, DWORD bufferSize)
{
	HKEY hKey;
	DWORD varType = REG_SZ;

	TCHAR kbdName[KL_NAMELENGTH];
	GetKeyboardLayoutName(kbdName);

	TCHAR kbdKeyPath[51 + KL_NAMELENGTH];
	wsprintf(kbdKeyPath,L"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%s", kbdName);

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR)kbdKeyPath, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        	return 0;

	if(RegQueryValueEx(hKey, L"Layout File", NULL, &varType, (LPBYTE)layoutFile, &bufferSize) != ERROR_SUCCESS)
		return 0;

	RegCloseKey(hKey);

	return 1;
}

typedef PKBDTABLES(CALLBACK* KbdLayerDescriptor)(VOID);

PVK_TO_WCHARS1 pVkToWchars1 = NULL;
PVK_TO_WCHARS2 pVkToWchars2 = NULL;
PVK_TO_WCHARS3 pVkToWchars3 = NULL;
PVK_TO_WCHARS4 pVkToWchars4 = NULL;
PVK_TO_WCHARS5 pVkToWchars5 = NULL;
PVK_TO_WCHARS6 pVkToWchars6 = NULL;
PVK_TO_WCHARS7 pVkToWchars7 = NULL;
PVK_TO_WCHARS8 pVkToWchars8 = NULL;
PVK_TO_WCHARS9 pVkToWchars9 = NULL;
PVK_TO_WCHARS10 pVkToWchars10 = NULL;


int VirtulKeyCodeToCharConverter::LoadKeyboardLayout()
{
	PKBDTABLES pKbd;
	HINSTANCE kbdLibrary;
	KbdLayerDescriptor pKbdLayerDescriptor = NULL;

	TCHAR layoutFile[MAX_PATH];
	if(!getKeyboardLayoutFile(layoutFile, sizeof(layoutFile)))
		return FALSE;

	TCHAR systemDirectory[MAX_PATH];
	GetSystemDirectory(systemDirectory, MAX_PATH);

	TCHAR kbdLayoutFilePath[MAX_PATH];
	wsprintf(kbdLayoutFilePath, L"%s\\%s", systemDirectory, layoutFile);

	kbdLibrary = LoadLibrary(kbdLayoutFilePath);

	pKbdLayerDescriptor = (KbdLayerDescriptor)GetProcAddress(kbdLibrary, "KbdLayerDescriptor");

	if(pKbdLayerDescriptor)
		pKbd = pKbdLayerDescriptor();
	else
		return FALSE;

	int i = 0;
	do
	{
		INIT_PVK_TO_WCHARS(i, 1)
		INIT_PVK_TO_WCHARS(i, 2)
		INIT_PVK_TO_WCHARS(i, 3)
		INIT_PVK_TO_WCHARS(i, 4)
		INIT_PVK_TO_WCHARS(i, 5)
		INIT_PVK_TO_WCHARS(i, 6)
		INIT_PVK_TO_WCHARS(i, 7)
		INIT_PVK_TO_WCHARS(i, 8)
		INIT_PVK_TO_WCHARS(i, 9)
		INIT_PVK_TO_WCHARS(i, 10)
		i++;
	}
	while(pKbd->pVkToWcharTable[i].cbSize != 0);

	m_pCharModifiers = pKbd->pCharModifiers;
	m_pDeadKey = pKbd->pDeadKey;
	m_kbdLibrary = kbdLibrary;

	return TRUE;
}

int VirtulKeyCodeToCharConverter::UnloadKeyboardLayout()
{
	BOOL success = FALSE;
	if(m_kbdLibrary)
	{
		
		free (m_pCharModifiers);
		m_pCharModifiers = NULL;
		free (m_pDeadKey);
		m_pDeadKey = NULL;
		success = FreeLibrary(m_kbdLibrary);
		m_kbdLibrary = NULL;
	}
	return success;
}

int VirtulKeyCodeToCharConverter::ConvertToWChar(int virtualKey, PTCHAR outputChar, PTCHAR deadChar)
{
	int i = 0;
	short state = 0;
	int shift = -1;
	int mod = 0;
	int charCount = 0;

	TCHAR baseChar;
	TCHAR diacritic;
	*outputChar = 0;

	int capsLock = (GetKeyState(VK_CAPITAL) & 0x1);

	do
	{
		state = GetAsyncKeyState(m_pCharModifiers->pVkToBit[i].Vk);

		if(m_pCharModifiers->pVkToBit[i].Vk == VK_SHIFT)
			shift = i + 1; // Get modification number for Shift key

		if(state & ~SHRT_MAX)
		{
			mod = i + 1; // There might be two modifiers on the same time. But here we always choose the last
		}
		i++;
	}
	while(m_pCharModifiers->pVkToBit[i].Vk != 0);

	SEARCH_VK_IN_CONVERSION_TABLE(1)
	SEARCH_VK_IN_CONVERSION_TABLE(2)
	SEARCH_VK_IN_CONVERSION_TABLE(3)
	SEARCH_VK_IN_CONVERSION_TABLE(4)
	SEARCH_VK_IN_CONVERSION_TABLE(5)
	SEARCH_VK_IN_CONVERSION_TABLE(6)
	SEARCH_VK_IN_CONVERSION_TABLE(7)
	SEARCH_VK_IN_CONVERSION_TABLE(8)
	SEARCH_VK_IN_CONVERSION_TABLE(9)
	SEARCH_VK_IN_CONVERSION_TABLE(10)

	if(*deadChar != 0) // I see dead characters...
	{
		i = 0;
		do
		{
			baseChar = (TCHAR)m_pDeadKey[i].dwBoth;
			diacritic = (TCHAR)(m_pDeadKey[i].dwBoth >> 16);

			if((baseChar == *outputChar) && (diacritic == *deadChar))
			{
				*deadChar = 0;
				*outputChar = (TCHAR)m_pDeadKey[i].wchComposed;
			}
			i++;
		}
		while(m_pDeadKey[i].dwBoth != 0);
	}

	return charCount;
}
