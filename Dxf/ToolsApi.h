#pragma once
class ToolsApi
{
public:
	ToolsApi();

	~ToolsApi();
	static char * unicodeToAnsi(TCHAR * wstr);

	static TCHAR * ansiToUnicode(char * str);

	static int getModleAddrByPid(DWORD process_id, LPCWSTR modle_name);
	/*ɾ������*/
	static bool deleteSelf();
	/*����ļ�*/
	static void clearFile(const char * f_path);
	/*����Ȩ��*/
	static bool WINAPI enablePrivileges();
	/*��ȡϵͳAPI��ַ*/
	static int getWinApiAddr(const char * modle_name, const char * fun_name);

	static byte * vectorByteToByte(vector<byte> bytes);

	vector<byte> byteToVectorByte(byte * bytes, int len);
};
