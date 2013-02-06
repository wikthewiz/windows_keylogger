#pragma once

#include "Timer.h"
#include "Runnible.h"
class Process : 
	public Runnible
{
public:
	Process(void);
	virtual ~Process(void);
	
	void SetIntervall(int tickInterval);
	int GetIntervall();

protected:
	virtual void TimerTick() = 0;

	void OnEnable();
	void OnDisable();

private:
	Timer *m_pTimer;
	static void OnTimerTick(Timer *, LPVOID);
	static const int DEFAULT_PROCESS_TICK_INTERVALL = 50;
};
