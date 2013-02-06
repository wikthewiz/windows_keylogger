#include "StdAfx.h"
#include "BufferWriter.h"



BufferWriter::BufferWriter(void)
{
}
BufferWriter *BufferWriter::m_pInstance = NULL;

BufferWriter* BufferWriter::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new BufferWriter();
	}
	return m_pInstance;
}


BufferWriter::~BufferWriter(void)
{
}


LRESULT CALLBACK BufferWriter::onIdleEvent(int code, WPARAM wParam, LPARAM lParam)
{
	return CallNextHookEx( BufferWriter::GetInstance()->m_hhk ,code,wParam,lParam);
}

void BufferWriter::OnEnable()
{
	m_hhk = SetWindowsHookEx(WH_FOREGROUNDIDLE,onIdleEvent, myInstance, NULL);
}

void BufferWriter::OnDisable()
{
}

void BufferWriter::SetBuffer(Buffer *buffer)
{
	m_pBuffer = buffer;
}
Buffer * BufferWriter::GetBuffer()
{
	return m_pBuffer;
}

void BufferWriter::SetWriter(Writer *writer)
{
	m_pWriter = writer;
}
Writer * BufferWriter::GetWriter()
{
	return m_pWriter;
}

void BufferWriter::writeToBuffer()
{
	int nrOfChars = m_pBuffer->GetNeededBufferSize();
	char* charBuffer = (char*)calloc(nrOfChars,sizeof(char));
	m_pBuffer->GetBuffer( charBuffer,nrOfChars );
	m_pWriter->Write(&charBuffer,nrOfChars);
}

