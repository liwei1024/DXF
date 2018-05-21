#include "stdafx.h"
#include "Activate.h"
#include "RemoteCall.h"
#include "Functions.h"
#include "Auto.h"
#include "BeastQuest.h"

HHOOK g_hHook = NULL;
DWORD g_dwMainThreadId;

LRESULT CALLBACK Keypress(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* p = (CWPSTRUCT*)lParam;
	if (nCode == HC_ACTION)
	{
		switch (wParam) //wParam中是消息ID
		{
		case WM_KEYDOWN:
		{
			//BeastQuest BQ;
			if (p->lParam == VK_HOME)
			{
				全局开关.自动剧情开关 = !全局开关.自动剧情开关;
				if (全局开关.自动剧情开关) {
					printf("自动剧情开\n");
				}
				else {
					printf("自动剧情关\n");
				}
			}
			if (p->lParam == VK_F1) {
				BeastQuest BQ;
				BQ.智能过图();
			}
			if (p->lParam == VK_F2) {
				Send_组包买物(18402);
			}
			if (p->lParam == VK_F3) {
				Send_测试组包();
			}
			if (p->lParam == VK_F4) {
				printf("%d\n",取角色疲劳());
			}

			

			//if (p->lParam == VK_F2) Send_存金入库(10000);
			if (p->lParam == 'l' || p->lParam == 'L')
			{
			}

			if (p->lParam == VK_ESCAPE)
			{
			}
		}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL WINAPI ConsoleCtrlhandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		UnhookWindowsHookEx(g_hHook);
		PostThreadMessage(g_dwMainThreadId, WM_QUIT, 0, 0);
	}
	return TRUE;
}

void KeypressStart()
{
	g_dwMainThreadId = GetCurrentThreadId();
	SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, Keypress, GetModuleHandle(NULL), NULL);
	if (g_hHook == NULL)
		printf("hook failure!");
}

void KeypressEnd()
{
	SetConsoleCtrlHandler(ConsoleCtrlhandler, FALSE);
}

void MainThread()
{
	//BeastQuest BQ;
	while (true)
	{
		if (全局开关.自动剧情开关) {
			自动剧情();
		}
		else if(全局开关.自动刷图开关) {
			自动刷图();
		}
		Sleep(100);
	}
}

void CreateMainThread()
{
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
}

void Activate()
{

	

	InitRemoteCall();

	CreateMainThread();

	KeypressStart();
}

void ProcessClose()
{
	KeypressEnd();
	if (ReHookMsg()) {
		pApi.freeAllAlloc();
	}
}