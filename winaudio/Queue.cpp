#include "StdAfx.h"
#include "Queue.h"

template class Queue<DWORD>;
template class Queue<TCHAR>;

template <class T>
Queue<T>::Queue():
	m_first(NULL),
	m_last(NULL),
	m_count(0)
{
	InitializeCriticalSection(&m_lock);
}
template <class T>
Queue<T>::~Queue()
{
	deleteAll();
	DeleteCriticalSection(&m_lock);
}
template <class T>
void Queue<T>::Enqueue(T element)
{
	QueueElement<T> *qElem = new QueueElement<T>(element);
	enqueue(qElem);
}

template <class T>
void Queue<T>::enqueue(QueueElement<T> *element)
{
	EnterCriticalSection(&m_lock);
	m_count++;
	if(m_last != NULL)
	{
		m_last->SetNext(element);
	}
	else
	{
		m_first = element;
	}
	m_last = element;
	LeaveCriticalSection(&m_lock);
}

template <class T>
T Queue<T>::Dequeue()
{
	QueueElement<T> *oldFirst = dequeue();
	if (oldFirst == NULL)
		return NULL;
	T t = oldFirst->GetElement();
	delete oldFirst;
	return t;
}
template <class T>
int Queue<T>::Dequeue(int count, T buffer[])
{
	int elemIndex;
	for(elemIndex = 0; elemIndex < count; elemIndex++)
	{
		QueueElement<T> *oldFirst = dequeue();
		
		if (oldFirst == NULL)
		{
			break;
		}

		buffer[elemIndex] = oldFirst->GetElement();
		delete oldFirst;
	}
	return elemIndex;
}

template <class T>
QueueElement<T>* Queue<T>::dequeue()
{
	EnterCriticalSection(&m_lock);
	if (m_first == NULL)
	{
		LeaveCriticalSection(&m_lock);
		return NULL;
	}
	QueueElement<T> *oldFirst = m_first;
	if ( m_first->GetNext() == NULL)
	{
		m_first = NULL;
		m_last = NULL;
	}
	else
	{
		m_first = m_first->GetNext();
		oldFirst->SetNext(NULL);
	}
	m_count--;
	LeaveCriticalSection(&m_lock);
	return oldFirst;
}
template <class T>
int Queue<T>::Count()
{
	int count = 0;
	EnterCriticalSection(&m_lock);
	count = m_count;
	LeaveCriticalSection(&m_lock);
	return count;
}


template <class T>
void Queue<T>::deleteAll()
{
	EnterCriticalSection(&m_lock);
	if (m_first != NULL)
	{
		QueueElement<T> *curElem = m_first;
		while(curElem != NULL)
		{
			QueueElement<T> *toBeDeleted = curElem;
			curElem = curElem->GetNext();
			delete toBeDeleted;
		}
		m_first = NULL;
		m_count = 0;
	}
	LeaveCriticalSection(&m_lock);
}