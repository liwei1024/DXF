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
		switch (wParam) //wParam������ϢID
		{
		case WM_KEYDOWN:
		{
			//BeastQuest BQ;
			if (p->lParam == VK_HOME)
			{
				ȫ�ֿ���.�Զ����鿪�� = !ȫ�ֿ���.�Զ����鿪��;
				if (ȫ�ֿ���.�Զ����鿪��) {
					printf("�Զ����鿪\n");
				}
				else {
					printf("�Զ������\n");
				}
			}
			if (p->lParam == VK_F1) {
				BeastQuest BQ;
				BQ.���ܹ�ͼ();
			}
			if (p->lParam == VK_F2) {
				Send_�������(18402);
			}
			if (p->lParam == VK_F3) {
				Send_�������();
			}
			if (p->lParam == VK_F4) {
				printf("%d\n",ȡ��ɫƣ��());
			}

			

			//if (p->lParam == VK_F2) Send_������(10000);
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
		if (ȫ�ֿ���.�Զ����鿪��) {
			�Զ�����();
		}
		else if(ȫ�ֿ���.�Զ�ˢͼ����) {
			�Զ�ˢͼ();
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