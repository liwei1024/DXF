#pragma once

struct MemoryStruct
{
	int first_addr;
	int size;
};
class ProcessApi
{
public:
	ProcessApi(LPCWSTR lpClassName = L"���³�����ʿ", LPCWSTR lpWindowName = L"���³�����ʿ");
	~ProcessApi();
	void InitHookRestore();
	/*�������ڴ�*/
	bool readMemory(int lpBaseAddress, LPVOID lpBuffer, int nSize);
	/*�������ֽ���*/
	byte readByte(int lpBaseAddress);
	/*�����̶�����*/
	short readShort(int lpBaseAddress);
	/*����������*/
	int readInteger(int lpBaseAddress);
	/*�����̳�����*/
	LONGLONG readLong(int lpBaseAddress);
	/*�����̵�������*/
	float readFloat(int lpBaseAddress);
	/*������˫������*/
	double readDouble(int lpBaseAddress);
	/*�������ֽڼ�*/
	vector<byte> readBytes(int lpBaseAddress, int len);
	/*�������ַ�������*/
	char * readString(int lpBaseAddress, int len);
	/*д�ڴ�*/
	bool writeMemory(int lpBaseAddress, LPCVOID lpBuffer, int nSize);
	/*д�ֽ���*/
	bool writeByte(int lpBaseAddress, byte lpBuffer);
	/*д������*/
	bool writeShort(int lpBaseAddress, short lpBuffer);
	/*д����*/
	bool writeInteger(int lpBaseAddress, int lpBuffer);
	/*д������*/
	bool writeLong(int lpBaseAddress, LONGLONG lpBuffer);
	/*д��������*/
	bool writeFloat(int lpBaseAddress, float lpBuffer);
	/*д˫������*/
	bool writeDouble(int lpBaseAddress, double lpBuffer);
	/*д�ַ���*/
	bool writeString(int lpBaseAddress, LPCWSTR lpBuffer);
	/*д�ֽڼ�*/
	bool writeBytes(int lpBaseAddress, vector<byte> bytes);
	/*�����ڴ�*/
	int allocMemory(const char * name, int size);
	/*�ͷ��ڴ�*/
	bool freeMemory(const char * name);
	/*�ͷ�����������ڴ�*/
	void freeAllAlloc();
	/*����Զ���߳�*/
	bool createThread(int lpThreadAttributes, int lpStartAddress, LPVOID lpParameter);
	/*ע��DLL*/
	void injectDll(LPCTSTR dll_path);
	/*���ھ��*/
	HWND hWnd = NULL;
	/*����ID*/
	DWORD ProcessId = NULL;
	/*���̾��*/
	HANDLE hProcess = NULL;
	/*������ڴ��ַ�������*/
	map<const char*, MemoryStruct>alloc_memory_map_box;

};

