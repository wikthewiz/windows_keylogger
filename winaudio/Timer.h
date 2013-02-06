#pragma once
#include "MainProcessHandler.h"


class Timer
{
public:
	Timer(long tick, void onTick(Timer *, LPVOID),LPVOID context);
	~Timer(void);

	void Enable();
	void Disable();
	bool IsEnabled();

	long GetTick();
	void SetTick(long ticksInMillis);

private:
	static DWORD WINAPI  TickThread(LPVOID lpParam);
	HANDLE m_hTickThread;
	bool m_isEnabled;
	void (*m_pOnTickCallback)(Timer*, LPVOID);
	bool m_exitThread;
	void dispose();
	static void waitForEnabled(Timer *);
	CRITICAL_SECTION g_criticalSection;
	long ticksInMillis;
	HANDLE m_hEnabledEvent;
	LPVOID m_context;
};

#if _DEBUG
class TimerTest
{
public:
	TimerTest()
	{
		timerCount = 0;
		g_hTimerEnded = CreateEvent(NULL,FALSE,FALSE,L"TimerTest event");
	}
	~TimerTest(void)
	{
		CloseHandle(g_hTimerEnded);
	}

	bool Start()
	{
		DWORD startTick = GetTickCount();
		m_t1 = createTimer(50);
		m_t1->Enable();
		WaitForSingleObject(g_hTimerEnded,INFINITE);
		DWORD endTick = GetTickCount();

		if (endTick - startTick >= 480)
		{
			Beep(1000,100);
		}
		timerCount = 0;
		m_t1->SetTick(1000);
		m_t1->Enable();
		startTick = GetTickCount();
		WaitForSingleObject(g_hTimerEnded,INFINITE);
		endTick = GetTickCount();

		if (endTick - startTick >= 9800)
		{
			Beep(1000,100);
		}
		
		m_t1->SetTick(1000);

		delete m_t1;

		return true;

	}

	static void OnTimerTick(Timer *timer,LPVOID context)
	{
		TimerTest *test = (TimerTest*)context;
		test->timerCount++;

		if (test->timerCount == 10)
		{
			Beep(500,100);
			timer->Disable();
			SetEvent(test->g_hTimerEnded);
		}
		else if(test->timerCount > 10)
		{
			Beep(500,300);
			Beep(500,300);
			Beep(500,300);
			
			Beep(500,700);
			Beep(500,700);
			Beep(1000,700);

			Beep(500,300);
			Beep(500,300);
			Beep(500,300);
		}
	}

	Timer *createTimer(int tickTime)
	{
		return new Timer(tickTime, &TimerTest::OnTimerTick, this);
	}
private:
	HANDLE g_hTimerEnded;
	Timer *m_t1;
	int timerCount;
};

#endif