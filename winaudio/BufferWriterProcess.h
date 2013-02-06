#pragma once

#include "Process.h"
#include "Writer.h"
#include "Buffer.h"
class BufferWriterProcess : public Process
{
public:
	BufferWriterProcess(Buffer*, Writer *);
	~BufferWriterProcess(void);
protected:
	 void TimerTick(); 
private:
	int static const BUFFER_WRITE_TIMER_INTERVALL = 30000;
	Writer *g_pWriter;
	Buffer *g_pBuffer;
	
};
