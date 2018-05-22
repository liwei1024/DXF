#include "stdafx.h"
#include "Test.h"

#define ≤‚ ‘≤Œ ˝ 0x0185E018

#define NtReadVirtualMemory(hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesRead){\
	FARPROC addr = GetProcAddress(GetModuleHandle(L"ntdll.dll"),"NtReadVirtualMemory");\
	__asm {\
		__asm push lpNumberOfBytesRead\
		__asm push nSize\
		__asm push lpBuffer\
		__asm push lpBaseAddress\
		__asm push hProcess\
		__asm mov eax, addr\
		__asm call eax\
	}\
}
void Test() {
	WriteProcessMemory(OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId()),(LPVOID)0x008938d0,(LPCVOID)10,4,0);
	return;
}

void CallTest(HANDLE  hProcess, LPCVOID  lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead) {
	
	NtReadVirtualMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}