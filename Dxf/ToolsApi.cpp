#include "stdafx.h"
#include "ToolsApi.h"




ToolsApi::ToolsApi()
{
}


ToolsApi::~ToolsApi()
{
}

char* ToolsApi::unicodeToAnsi(TCHAR* wstr)
{
	if (!wstr)
		return NULL;
	int strlen = ::WideCharToMultiByte(CP_ACP, NULL, wstr, wcslen(wstr), NULL, 0, NULL, FALSE);
	char* str = new char[strlen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, wstr, wcslen(wstr), str, strlen, NULL, FALSE);
	str[strlen] = '\0';
	return str;
}

TCHAR* ToolsApi::ansiToUnicode(char* str)
{
	if (!str)
		return NULL;
	int wcslen = ::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), NULL, 0);
	TCHAR *wstr = new TCHAR[wcslen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), wstr, wcslen);
	wstr[wcslen] = '\0';
	return wstr;
}

int ToolsApi::getModleAddrByPid(DWORD process_id, LPCWSTR modle_name)
{
	HANDLE        hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 1. ����һ��ģ����صĿ��վ��
	hModuleSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,  // ָ�����յ�����
		process_id);            // ָ������
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return false;
	// 2. ͨ��ģ����վ����ȡ��һ��ģ����Ϣ
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return false;
	}
	// 3. ѭ����ȡģ����ϢszExePath
	do {
		//_tprintf(TEXT("%s -- %s"), me32.szModule, modle_name);
		if (wcscmp(me32.szModule, modle_name) == 0) {
			//printf("ģ����%x  ", me32.hModule);
			//printf("���ػ�ַ%x  ", me32.modBaseAddr);
			return (int)me32.modBaseAddr;
		}
	} while (Module32Next(hModuleSnap, &me32));
	// 4. �رվ�����˳�����
	CloseHandle(hModuleSnap);
	return 0;
}

bool ToolsApi::deleteSelf()
{
	char FileName[MAX_PATH];
	memset(FileName, 0, MAX_PATH);

	//��ȡ�ļ�·��
	GetModuleFileName(NULL, (LPWSTR)FileName, MAX_PATH);
	char *NewFileName;
	NewFileName = new char[MAX_PATH];
	memset(NewFileName, 0, MAX_PATH);

	//�����ļ�����
	SetFileAttributes((LPCWSTR)NewFileName, FILE_ATTRIBUTE_NORMAL);

	//����ֱ��ɾ��
	if (DeleteFile((LPCWSTR)FileName))//��ֱ��ɾ�����
	{
		delete[] NewFileName;
		return false;
	}

	//����APIһ�������ن���
	wsprintf((LPWSTR)NewFileName, L"%C:\\\0", FileName[0]);
	CreateDirectory((LPCWSTR)NewFileName, NULL);
	SetFileAttributes((LPCWSTR)NewFileName, FILE_ATTRIBUTE_HIDDEN);
	wsprintf((LPWSTR)NewFileName, L"%C:\\Windows ����������\0", FileName[0], GetTickCount());
	SetFileAttributes((LPCWSTR)NewFileName, FILE_ATTRIBUTE_NORMAL);
	DeleteFile((LPCWSTR)NewFileName);
	if (!MoveFileEx((LPCWSTR)FileName, (LPCWSTR)NewFileName, MOVEFILE_REPLACE_EXISTING))
	{
		delete[] NewFileName;
		return false;//�����˾ͷ���
	}
	MoveFileEx((LPCWSTR)NewFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	SetFileAttributes((LPCWSTR)NewFileName, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
	delete[] NewFileName;
	return true;
}

void ToolsApi::clearFile(const char*f_path)
{
	ofstream fileout(f_path, ios::trunc);//ios::trunc�����ԭ�ļ�����,�ɲ�д,Ĭ�Ͼ�����
	if (!fileout) {
		//cout << "Create file failure...\n";
		printf("Create file failure...\n");
	}
	fileout.close();
	return;
}

bool ToolsApi::enablePrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

int ToolsApi::getWinApiAddr(LPCSTR modle_name, LPCSTR fun_name)
{
	return (int)GetProcAddress(GetModuleHandleA(modle_name), fun_name);
}

byte* ToolsApi::vectorByteToByte(vector<byte> vector_bytes)
{
	int size = vector_bytes.size();
	byte *bytes = new byte[size];
	for (int i = 0; i < size; i++)
	{
		bytes[i] = vector_bytes[i];
	}
	return bytes;
}

vector<byte> ToolsApi::byteToVectorByte(byte * bytes, int len)
{
	vector<byte> vectorByte;
	vectorByte.resize(len);
	for (int i = 0; i < len; i++)
	{
		vectorByte[i] = bytes[i];
	}
	delete[]bytes;
	return vectorByte;
}
