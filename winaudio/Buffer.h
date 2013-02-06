#pragma once

#include "Queue.h"

class Buffer
{
public:
	Buffer(void);
	virtual ~Buffer(void);

	void Add(TCHAR c);
	void GetBuffer(TCHAR buffer[], int count);
	int GetNeededBufferSize();

private:;
	Queue<TCHAR> *m_buffer;
	CRITICAL_SECTION m_writeLock;
};

#if _DEBUG

#define NR_OF_CHARS_IN_ORIGINAL  34
#define LENGTH_OF_ORIGINAL  35

static const TCHAR g_originalchar[] =  {'A','b','C','d','E','F','G','H','i','j','k','l','m','n','o','p','Q','e','s','t','u','v','x','Y','z','1','2','3','4','5','6','7','8','9','\0'};
static TCHAR g_readChar[LENGTH_OF_ORIGINAL];
static bool g_keepRunning = true;
class BufferTester
{
public:
	
	//static TCHAR[] g_readChar;
	BufferTester(void)
	{
	}

	bool TestReadWrite()
	{
		srand(time_t(NULL));
		int count;
		bool errorSuccess = true;
		for (int i = 0; i < 100; i++)
		{
			Buffer *buffer = new Buffer();
			g_readChar[0] = L'\0';
			g_keepRunning = true;
			HANDLE hReaderThread = ::CreateThread(NULL,NULL,&BufferTester::ReadThread,buffer, NULL ,NULL);
			HANDLE hWriterThread = ::CreateThread(NULL,NULL,&BufferTester::WriteThread,buffer, NULL ,NULL);
			//SetThreadPriority(hWriterThread,THREAD_PRIORITY_ABOVE_NORMAL);

			WaitForSingleObject(hWriterThread,INFINITE);
			g_keepRunning = false;

			//ReadThread(buffer);
			WaitForSingleObject(hReaderThread,INFINITE);

			if (wcscmp(g_originalchar,g_readChar))
			{

				delete buffer;
				
				count = i;
				errorSuccess = false;;
				break;
			}

			delete buffer;
		}
		return errorSuccess;
	}

	bool RunTest()
	{
		if (!TestReadWrite())
			return false;
		return true;
	}


	static DWORD WINAPI WriteThread(LPVOID lpParam)
	{
		Buffer *buffer = (Buffer*)lpParam;
		for(int i = 0; i < LENGTH_OF_ORIGINAL; i++)
		{
			buffer->Add(g_originalchar[i]);
			int rVal = rand();
			if ( !(rVal  % 13) ) Sleep(rand() % 400) ;
		}

		return 0;
	}
	
	static DWORD WINAPI ReadThread(LPVOID lpParam)
	{
		Buffer *buffer = (Buffer*)lpParam;
		do
		{
			writeToReadChars(buffer);
		}while(g_keepRunning);
		writeToReadChars(buffer);
		return 0;
	}

	static int writeToReadChars(Buffer *buffer)
	{
		int neededBufferSize = buffer->GetNeededBufferSize();
		if (neededBufferSize > 1)
		{
			TCHAR *strBuffer = (TCHAR*)malloc(neededBufferSize * sizeof(TCHAR));
			buffer->GetBuffer(strBuffer,neededBufferSize);
			wcscat_s(g_readChar,LENGTH_OF_ORIGINAL,strBuffer);
			free(strBuffer);
		}
		return neededBufferSize - 1;
	}
};
#endif