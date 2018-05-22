#include "stdafx.h"
#include "ProcessApi.h"
#include "ToolsApi.h"



ProcessApi::ProcessApi(LPCWSTR lpClassName, LPCWSTR lpWindowName)
{
	if (hWnd == 0) hWnd = ::FindWindow(lpClassName, lpWindowName);

	if (ProcessId == 0) ::GetWindowThreadProcessId(hWnd, &ProcessId);

	if (hProcess == 0) hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, ProcessId);

	//InitHookRestore();

}

ProcessApi::~ProcessApi()
{
	//freeAllAlloc();
	//printf("退出清理完毕...\n");
	//system("pause");
}

void ProcessApi::InitHookRestore()
{
	//ntdll.LdrInitializeThunk 还原钩子
	vector<byte> buff;
	int addr;
	/*addr = ToolsApi::getWinApiAddr("ntdll.dll", "LdrInitializeThunk");
	buff = readBytes(addr, 5);
	if (buff[0] != 0x8b) {
		writeBytes(addr, { 0x8b, 0xff, 0x55, 0x8b, 0xec });
	}*/
	addr = ToolsApi::getWinApiAddr("ntdll.dll", "NtQueryVirtualMemory");
	buff = readBytes(addr, 5);
	if (buff[0] == 0xe9) {
		writeBytes(addr, {0xb8,0x23,0,0,0 });
	}
}



bool ProcessApi::readMemory(int lpBaseAddress, LPVOID lpBuffer, int nSize)
{
	/*int addr = ToolsApi::getWinApiAddr((LPCSTR)"ntdll.dll", (LPCSTR)"NtQueryVirtualMemory");
	SIZE_T lpNumberOfBytesRead;
	vector<byte> buff;
	if (addr) {
		byte * bytes = new byte[5];
		memset(bytes, 0, 5);
		if (ReadProcessMemory(hProcess, (LPCVOID)addr, bytes, 5, &lpNumberOfBytesRead)) {
			buff = ToolsApi::byteToVectorByte(bytes, 5);
			if (writeBytes(addr, { 0xb8,0x23,0,0,0 })) {
				if (!ReadProcessMemory(hProcess, (LPCVOID)lpBaseAddress, lpBuffer, (SIZE_T)nSize, &lpNumberOfBytesRead)) {
					printf("读取 %x 内存时失败！\n", lpBaseAddress);
					return false;
				}
				if (lpNumberOfBytesRead != nSize) {
					printf("读取 %x 内存时实际读取的长度与要读取的长度不一致！\n", lpBaseAddress);
					return false;
				}
				writeBytes(addr, buff);
				return true;
			}
		}
	}
	return false;*/
	SIZE_T lpNumberOfBytesRead;
	if (!ReadProcessMemory(hProcess, (LPCVOID)lpBaseAddress, lpBuffer, (SIZE_T)nSize, &lpNumberOfBytesRead)) {
		printf("读取 %x 内存时失败！\n", lpBaseAddress);
		return false;
	}
	if (lpNumberOfBytesRead != nSize) {
		printf("读取 %x 内存时实际读取的长度与要读取的长度不一致！\n", lpBaseAddress);
		return false;
	}
	return true;
}

byte ProcessApi::readByte(int lpBaseAddress)
{
	byte lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

short ProcessApi::readShort(int lpBaseAddress)
{
	short lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

int ProcessApi::readInteger(int lpBaseAddress)
{
	int lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

LONGLONG ProcessApi::readLong(int lpBaseAddress)
{
	LONGLONG lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

float ProcessApi::readFloat(int lpBaseAddress)
{
	float lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

double ProcessApi::readDouble(int lpBaseAddress)
{
	double lpBuffer;
	readMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
	return lpBuffer;
}

vector<byte> ProcessApi::readBytes(int lpBaseAddress, int len)
{
	byte * lpBuffer;
	lpBuffer = new byte[len];
	memset(lpBuffer, 0, len);
	readMemory(lpBaseAddress, lpBuffer, len);
	vector<byte>  result;
	result.resize(len);
	for (int i = 0; i < len; i++)
	{
		result[i] = lpBuffer[i];
	}
	delete[]lpBuffer;
	return result;
}

char* ProcessApi::readString(int lpBaseAddress, int len)
{
	TCHAR *lpBuffer;
	len = len * 2 + 2;
	lpBuffer = new TCHAR[len];
	readMemory(lpBaseAddress, lpBuffer, len);
	char * str = ToolsApi::unicodeToAnsi(lpBuffer);
	return str;
}

bool ProcessApi::writeMemory(int lpBaseAddress, LPCVOID lpBuffer, int nSize)
{
	SIZE_T lpNumberOfBytesRead;
	if (!WriteProcessMemory(hProcess, (LPVOID)lpBaseAddress, lpBuffer, (SIZE_T)(nSize), &lpNumberOfBytesRead)) {
		printf("写入 %x 内存时失败！\n", lpBaseAddress);
		return false;
	}
	if (lpNumberOfBytesRead != nSize) {
		printf("写入 %x 内存时实际写入的长度与要写入的长度不一致！\n", lpBaseAddress);
		return false;
	}
	return true;
}

bool ProcessApi::writeByte(int lpBaseAddress, byte lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeShort(int lpBaseAddress, short lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeInteger(int lpBaseAddress, int lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeLong(int lpBaseAddress, LONGLONG lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeFloat(int lpBaseAddress, float lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeDouble(int lpBaseAddress, double lpBuffer)
{
	return writeMemory(lpBaseAddress, &lpBuffer, sizeof(lpBuffer));
}

bool ProcessApi::writeString(int lpBaseAddress, LPCWSTR lpBuffer)
{
	int len = wcslen(lpBuffer) * 2 + 2;
	return writeMemory(lpBaseAddress, (LPCVOID)lpBuffer, len);
}

bool ProcessApi::writeBytes(int lpBaseAddress, vector<byte> bytes)
{
	int size = bytes.size();
	byte *lpBuffer = new byte[size];
	for (int i = 0; i < size; i++)
	{
		lpBuffer[i] = bytes[i];
	}
	bool result = writeMemory(lpBaseAddress, lpBuffer, size);
	delete[]lpBuffer;
	return result;
}

int ProcessApi::allocMemory(const char *name, int size)
{
	MemoryStruct memoryStruct = alloc_memory_map_box[name];
	if (memoryStruct.size == size) {
		return memoryStruct.first_addr;
	}
	else {
		memoryStruct.first_addr = (int)VirtualAllocEx(hProcess, NULL, (SIZE_T)size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!memoryStruct.first_addr) {
			printf("%s 分配内存失败!\n", name);
			return -1;
		}
		memoryStruct.size = size;
		alloc_memory_map_box[name] = memoryStruct;
		return memoryStruct.first_addr;
	}
}

bool ProcessApi::freeMemory(const char *name)
{
	MemoryStruct memoryStruct = alloc_memory_map_box[name];
	if (VirtualFreeEx(hProcess, (LPVOID)memoryStruct.first_addr, 0, MEM_RELEASE))
	{
		printf("%s 释放成功!\n", name);
		return true;
	}
	else {
		printf("%s 释放失败!\n", name);
		return false;
	}
}

void ProcessApi::freeAllAlloc()
{
	map<const char*, MemoryStruct > ::iterator mapi;
	mapi = alloc_memory_map_box.begin();
	MemoryStruct tmp;
	while (mapi != alloc_memory_map_box.end())
	{
		tmp = mapi->second;
		VirtualFreeEx(hProcess, (LPVOID)tmp.first_addr, 0, MEM_RELEASE);
		mapi++;
	}
}

bool ProcessApi::createThread(int lpThreadAttributes, int lpStartAddress, LPVOID lpParameter = NULL)
{
	if (!CreateRemoteThreadEx(hProcess, (LPSECURITY_ATTRIBUTES)lpThreadAttributes, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpParameter, 0, NULL, 0)) {
		printf("创建线程失败！");
		return false;
	}
	return true;
}

void ProcessApi::injectDll(LPCTSTR dll_path)
{
	int addr = allocMemory(__FUNCTION__, MAX_PATH);
	int fun_add = ToolsApi::getWinApiAddr("kernel32.dll", "LoadLibraryW");
	bool result = this->writeMemory((int)addr, (PVOID)dll_path, (_tcslen(dll_path) + 1) * sizeof(dll_path[0]));
	if (result == false) {
		printf("InjectDll Fail!\n");
		system("pause");
		return;
	}
	if (createThread(fun_add, addr)) {
		printf("注入成功！\n");
	}
	else {
		printf("创建线程失败！\n");
		system("pause");
	}
}

