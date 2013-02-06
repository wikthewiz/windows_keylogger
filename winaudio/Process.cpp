#include "StdAfx.h"
#include "Process.h"

Process::Process(void)
{
	m_pTimer = new Timer(DEFAULT_PROCESS_TICK_INTERVALL, &Process::OnTimerTick,this);
}

Process::~Process(void)
{
	if (m_pTimer->IsEnabled())
	{
		m_pTimer->Disable();
	}
	delete m_pTimer;
}

void Process::OnTimerTick(Timer *timer, LPVOID context)
{
	Process *process = (Process*)context;
	process->TimerTick();
}

void Process::OnEnable()
{
	m_pTimer->Enable();
}
void Process::OnDisable()
{
	m_pTimer->Disable();
}

void Process::SetIntervall(int tickIntervall)
{
	m_pTimer->SetTick(tickIntervall);
}
int Process::GetIntervall()
{
	return m_pTimer->GetTick();
}