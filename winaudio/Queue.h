#pragma once


template <class T> class QueueElement
{
public:
	QueueElement(T elem)
	{
		m_elem = elem;
		m_next = NULL;
	}

	T GetElement()
	{
		return m_elem;
	}

	QueueElement<T> *GetNext()
	{
		return m_next;
	}

	void SetNext(QueueElement<T> *next)
	{
		m_next = next;
	}

	void DeleteElement()
	{
		if (m_elem != NULL)
		{
			delete m_elem;
		}
	}

private:
	T m_elem;
	QueueElement<T> *m_next;
};

template <class T> class Queue
{
public:
	Queue();
	~Queue();

	void Enqueue(T element);
	T Dequeue();
	int Dequeue(int count, T buffer[]); 
	int Count();


private:
	QueueElement<T>* dequeue();
	void enqueue(QueueElement<T> *element);
	void deleteAll();
	QueueElement<T> *m_first;
	QueueElement<T> *m_last;
	int m_count;
	CRITICAL_SECTION m_lock;
};



#if _DEBUG

static bool EnqueueTest(Queue<DWORD> *q)
{
	for (DWORD i = 0; i < 1000; i++)
	{
		q->Enqueue(i);
	}
	return q->Count() == 1000;
}
static bool DequeueTest(Queue<DWORD> *q)
{
	bool success = true;
	for (DWORD i = 0; i < 1000; i++)
	{
		success = (i == q->Dequeue() && success);
	}
	return success && q->Count() == 0;
}
static bool DestrucorTest()
{
	Queue<DWORD> q;
	for (DWORD i = 0; i < 100000; i++)
	{
		q.Enqueue(i);
	}
	return true;
}
static bool WrongValuesTest()
{
	Queue<DWORD> q;
	bool success =  q.Dequeue() == NULL;
	for (DWORD i = 0; i < 5; i++)
	{
		q.Enqueue(i);
	}
	for (DWORD j = 0; j < 7; j++)
	{
		if (j < 5)
		{
			success = j == q.Dequeue() && success;
		}
		else
		{
			success =  q.Dequeue() == NULL && success;
		}
	}
	return success;
}

static bool GroupDequeueTest()
{
	Queue<DWORD> q;
	bool success =  q.Count() == 0;
	for (DWORD i = 0; i < 1000; i++)
	{
		q.Enqueue(i);
	}

	int count = q.Count();
	DWORD *buffer = (DWORD*)calloc(count, sizeof(DWORD));
	
	success = q.Dequeue(count,buffer) == 1000 && success;

	for(DWORD j = 0; j < 1000; j++)
	{
		 success = *(buffer + j) == j && success;
	}
	
	success = q.Count() == 0 && success;

	// test that it return 0 when getting from empty queue
	success = q.Dequeue(10, buffer) == 0 && success;

	// test that it doesn't impact current buffer when getting from empty queue
	for (DWORD g = 0; g < 1000; g++)
	{
		success = *(buffer + g) == g && success;
	}

	delete buffer;
	return success;
}


static bool g_keepRunningQ;
static bool g_readThreadSuccess;


static DWORD WINAPI WriteThread(LPVOID lpParam)
{
	Queue<DWORD> *q = (Queue<DWORD>*)lpParam;
	for(DWORD i = 0; i < 1000; i++)
	{
		q->Enqueue(i);
		int rVal = rand();
		if ( !(rVal  % 13) ) 
			Sleep(rand() % 100) ;
	}

	return 0;
}

static DWORD WINAPI ReadThread(LPVOID lpParam)
{
	Queue<DWORD> *q = (Queue<DWORD>*)lpParam;
	int totCount = 0;
	bool success = true;
	do
	{
		DWORD count = (DWORD)q->Count();
		DWORD *buffer = (DWORD*)calloc(count, sizeof(DWORD));

		success = q->Dequeue(count,buffer) == count && success;

		for(DWORD j = 0; j < count; j++)
		{
			success = *(buffer + j) == (totCount  + j) && success;
		}
		delete buffer;
		totCount += count;

	}while(g_keepRunningQ || q->Count() > 0);

	g_readThreadSuccess = success;

	return 0;
}

static bool ThreadSaftyTest()
{
	srand(time_t(NULL));
	Queue<DWORD> q;
	for (int i = 0; i < 100; i++)
	{
		g_keepRunningQ = true;
		g_readThreadSuccess = false;
		HANDLE hReaderThread = ::CreateThread(NULL,NULL,&ReadThread,&q, NULL ,NULL);
		HANDLE hWriterThread = ::CreateThread(NULL,NULL,&WriteThread,&q, NULL ,NULL);
		//SetThreadPriority(hWriterThread,THREAD_PRIORITY_ABOVE_NORMAL);

		WaitForSingleObject(hWriterThread,INFINITE);
		g_keepRunningQ = false;

		//ReadThread(buffer);
		WaitForSingleObject(hReaderThread,INFINITE);

		if (!g_readThreadSuccess || q.Count() != 0)
		{
			break;
		}
	}
	return g_readThreadSuccess;
}

static void QueueTest()
{
	Queue<DWORD> q;
	if (!EnqueueTest(&q))
	{
		return;
	}
	if (!DequeueTest(&q))
	{
		return;
	}
	DestrucorTest();

	if(!WrongValuesTest())
	{
		return;
	}

	if (!GroupDequeueTest())
	{
		return;
	}
	if (!ThreadSaftyTest())
	{
		return;
	}
}
#endif