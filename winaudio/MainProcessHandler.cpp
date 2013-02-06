#include "stdafx.h"
#include "MainProcessHandler.h"

MainProcessHandler *MainProcessHandler::m_instance = NULL;


MainProcessHandler* MainProcessHandler::GetInstance()
{
	if (MainProcessHandler::m_instance == NULL)
	{
		MainProcessHandler::m_instance = new MainProcessHandler();
	}
	return MainProcessHandler::m_instance;
}

MainProcessHandler::MainProcessHandler()
{
	m_hMainThreadId = GetCurrentThreadId();
}

void MainProcessHandler::Run()
{
	// TODO: Place code here.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!handleMsg(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	delete m_instance;
}

void MainProcessHandler::PostToMainThread(DWORD wParam, DWORD lParam)
{
	PostThreadMessage (m_hMainThreadId,wParam, wParam,lParam);
}
bool MainProcessHandler::handleMsg(MSG *msg)
{
	if(msg->message == TIMER_TEST_READY)
	{
		PostQuitMessage(0);
	}
	return true;
}
void MainProcessHandler::Exit()
{
	int exitCode = 100;
	PostToMainThread(exitCode, 0);
}