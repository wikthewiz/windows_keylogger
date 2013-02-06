#include "StdAfx.h"
#include "Timer.h"

Timer::Timer(long tick, void onTick(Timer *, LPVOID), LPVOID context):
	m_context(context)
{
	InitializeCriticalSection(&g_criticalSection);
	m_pOnTickCallback = onTick;
	m_isEnabled = false;
	m_exitThread = false;
	ticksInMillis = tick;
	m_hTickThread = CreateThread(NULL, NULL, &Timer::TickThread, this , 0, NULL);
	m_hEnabledEvent = CreateEvent(NULL,FALSE,FALSE,L"EnabledEvent");
}

Timer::~Timer(void)
{
	this->dispose();
}

void Timer::Enable()
{
	EnterCriticalSection(&g_criticalSection);
	if (!m_isEnabled)
	{
		m_isEnabled = true;
		SetEvent(m_hEnabledEvent);
	}
	LeaveCriticalSection(&g_criticalSection);
}

void Timer::Disable()
{
	EnterCriticalSection(&g_criticalSection);
	m_isEnabled = false;
	LeaveCriticalSection(&g_criticalSection);
}
bool Timer::IsEnabled()
{
	bool isEnabled;
	EnterCriticalSection(&g_criticalSection);
	isEnabled = m_isEnabled;
	LeaveCriticalSection(&g_criticalSection);
	return isEnabled;
}

void Timer::dispose()
{
	m_exitThread = true;
	Enable();
	WaitForSingleObject(m_hTickThread,INFINITE);
	CloseHandle(m_hTickThread);
	DeleteCriticalSection(&g_criticalSection);
}

long Timer::GetTick()
{
	long tick;
	EnterCriticalSection(&g_criticalSection);
	tick = ticksInMillis;
	LeaveCriticalSection(&g_criticalSection);
	return tick;
}
void Timer::SetTick(long ticksInMillis)
{
	EnterCriticalSection(&g_criticalSection);
	this->ticksInMillis = ticksInMillis;
	LeaveCriticalSection(&g_criticalSection);
}

void Timer::waitForEnabled(Timer *timer)
{
	while(!timer->IsEnabled())
	{
		WaitForSingleObject(timer->m_hEnabledEvent,INFINITE);
	}
}
DWORD WINAPI Timer::TickThread( LPVOID lpParam )
{
	Timer *timer = (Timer*)lpParam;
	waitForEnabled(timer);
	while(!timer->m_exitThread)
	{
		Sleep( timer->GetTick() );
		timer->m_pOnTickCallback(timer,timer->m_context);
		Timer::waitForEnabled(timer);
	}
	SetEvent(timer->m_hTickThread);
	return 0;
}
