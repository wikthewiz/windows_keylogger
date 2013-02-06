#include "StdAfx.h"
#include "Buffer.h"

Buffer::Buffer(void):m_buffer(new Queue<TCHAR>()){}

Buffer::~Buffer(void)
{
	if (m_buffer != NULL)
	{
		delete m_buffer;
		m_buffer = NULL;
	}
}

void Buffer::Add(TCHAR c)
{
	m_buffer->Enqueue(c);
}

int Buffer::GetNeededBufferSize()
{
	return m_buffer->Count() + 1;
}

void Buffer::GetBuffer(TCHAR buffer[], int count)
{ 
	m_buffer->Dequeue(count - 1,buffer);
	buffer[count - 1] = L'\0';
}
