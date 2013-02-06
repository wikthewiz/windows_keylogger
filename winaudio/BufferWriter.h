#pragma once
#include "runnible.h"
#include "Buffer.h"
#include "Writer.h"

class BufferWriter :
	public Runnible
{
public:
	
	static BufferWriter* GetInstance();
	~BufferWriter(void);

	void SetBuffer(Buffer*);
	Buffer * GetBuffer();

	void SetWriter(Writer *);
	Writer * GetWriter();

protected:
	
	void OnEnable();
	void OnDisable();

private:
	BufferWriter(void);

	static LRESULT CALLBACK onIdleEvent(int code, WPARAM wParam, LPARAM lParam);

	static BufferWriter *m_pInstance;

	void writeToBuffer();

	HHOOK m_hhk;
	Buffer *m_pBuffer;
	Writer *m_pWriter;

};
