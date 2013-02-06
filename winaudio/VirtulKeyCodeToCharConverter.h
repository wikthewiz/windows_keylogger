#pragma once

#include "kbdext.h"
class VirtulKeyCodeToCharConverter
{
public:
	VirtulKeyCodeToCharConverter(void);
	~VirtulKeyCodeToCharConverter(void);

	int LoadKeyboardLayout();
	int UnloadKeyboardLayout();
	int ConvertToWChar(int virtualKey, PTCHAR out, PTCHAR buffer);
private:
	int getKeyboardLayoutFile(TCHAR* layoutFile, DWORD bufferSize);

	PMODIFIERS m_pCharModifiers;
	PDEADKEY m_pDeadKey;
	HINSTANCE m_kbdLibrary;

};

#ifdef _DEBUG

static bool RunVirtualKeyCodeConverterTest()
{
	/*VirtulKeyCodeToCharConverter converter;
	TCHAR fileName[MAX_PATH];
	if (!converter.getKeyboardLayoutFile((TCHAR*)fileName,MAX_PATH))
	{
		return false;
	}

	return wcsnlen(fileName,MAX_PATH) > 0;*/
	return TRUE;

}

#endif
