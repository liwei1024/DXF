// Dxf.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Activate.h"
#include "Test.h"

int main()
{
	int buffer;
	SIZE_T lpNumberOfBytesRead = 0;
	CallTest(pApi.hProcess,(LPCVOID)0x0185E018, &buffer,sizeof(buffer),&lpNumberOfBytesRead);
	printf("%d\n",buffer);
	system("pause");
	ToolsApi::deleteSelf();
	system("color 0b");
	SetConsoleTitle(L"C:\\Windows\\system32\\svchost.exe");
	Activate();//激活
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ProcessClose();
    return 0;
}

