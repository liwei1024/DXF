#pragma once

struct MemoryStruct
{
	int first_addr;
	int size;
};
class ProcessApi
{
public:
	ProcessApi(LPCWSTR lpClassName = L"地下城与勇士", LPCWSTR lpWindowName = L"地下城与勇士");
	~ProcessApi();
	void InitHookRestore();
	/*读进程内存*/
	bool readMemory(int lpBaseAddress, LPVOID lpBuffer, int nSize);
	/*读进程字节型*/
	byte readByte(int lpBaseAddress);
	/*读进程短整型*/
	short readShort(int lpBaseAddress);
	/*读进程整型*/
	int readInteger(int lpBaseAddress);
	/*读进程长整型*/
	LONGLONG readLong(int lpBaseAddress);
	/*读进程单浮点型*/
	float readFloat(int lpBaseAddress);
	/*读进程双浮点型*/
	double readDouble(int lpBaseAddress);
	/*读进程字节集*/
	vector<byte> readBytes(int lpBaseAddress, int len);
	/*读进程字符串类型*/
	char * readString(int lpBaseAddress, int len);
	/*写内存*/
	bool writeMemory(int lpBaseAddress, LPCVOID lpBuffer, int nSize);
	/*写字节型*/
	bool writeByte(int lpBaseAddress, byte lpBuffer);
	/*写短整型*/
	bool writeShort(int lpBaseAddress, short lpBuffer);
	/*写整型*/
	bool writeInteger(int lpBaseAddress, int lpBuffer);
	/*写长整型*/
	bool writeLong(int lpBaseAddress, LONGLONG lpBuffer);
	/*写单浮点型*/
	bool writeFloat(int lpBaseAddress, float lpBuffer);
	/*写双浮点型*/
	bool writeDouble(int lpBaseAddress, double lpBuffer);
	/*写字符串*/
	bool writeString(int lpBaseAddress, LPCWSTR lpBuffer);
	/*写字节集*/
	bool writeBytes(int lpBaseAddress, vector<byte> bytes);
	/*申请内存*/
	int allocMemory(const char * name, int size);
	/*释放内存*/
	bool freeMemory(const char * name);
	/*释放所有申请的内存*/
	void freeAllAlloc();
	/*创建远程线程*/
	bool createThread(int lpThreadAttributes, int lpStartAddress, LPVOID lpParameter);
	/*注入DLL*/
	void injectDll(LPCTSTR dll_path);
	/*窗口句柄*/
	HWND hWnd = NULL;
	/*进程ID*/
	DWORD ProcessId = NULL;
	/*进程句柄*/
	HANDLE hProcess = NULL;
	/*申请的内存地址存放容器*/
	map<const char*, MemoryStruct>alloc_memory_map_box;

};

