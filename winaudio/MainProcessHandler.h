#pragma once
class MainProcessHandler
{
public:
	void Run();
	void PostToMainThread(DWORD wParam, DWORD lParam);
	void Exit();
	static MainProcessHandler * GetInstance();
private:
	static MainProcessHandler *m_instance;
	bool handleMsg(MSG *);
	DWORD m_hMainThreadId;
	MainProcessHandler();
};


enum Messages
{
	TIMER_TEST_READY = WM_USER + 1,
};