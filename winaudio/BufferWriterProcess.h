#pragma once

#include "Process.h"
#include "Writer.h"
#include "Buffer.h"
#include "Crypto.h"

class BufferWriterProcess : public Process
{
public:
	BufferWriterProcess(Buffer*, Writer *);
	virtual ~BufferWriterProcess(void);
	void EnableEncryption();
	void DisableEncryption();
protected:
	 void TimerTick(); 
private:
	Buffer *g_pBuffer;
	Writer *g_pWriter;
	bool m_enableEncryption;
	Crypto *m_crypto;
	int static const BUFFER_WRITE_TIMER_INTERVALL = 30000;
	
};
